// SPDX-License-Identifier: GPL-2.0-only
/*
 * cyttsp_i2c.c
 * Cypress TrueTouch(TM) Standard Product (TTSP) I2C touchscreen driver.
 * For use with Cypress  Txx4xx parts.
 * Supported parts include:
 * TMA4XX
 * TMA1036
 *
 * Copyright (C) 2009, 2010, 2011 Cypress Semiconductor, Inc.
 * Copyright (C) 2012 Javier Martinez Canillas <javier@dowhile0.org>
 * Copyright (C) 2013 Cypress Semiconductor
 *
 * Contact Cypress Semiconductor at www.cypress.com <ttdrivers@cypress.com>
 */

#include "cyttsp4_core.h"
#include "cyttsp4_devtree.h"

#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/of_device.h>

#define CYTTSP4_I2C_DATA_SIZE	(3 * 256)

static const struct cyttsp4_bus_ops cyttsp4_i2c_bus_ops = {
	.bustype	= BUS_I2C,
	.write		= cyttsp_i2c_write_block_data,
	.read           = cyttsp_i2c_read_block_data,
};

static struct of_device_id cyttsp4_i2c_of_match[] = {
	{ .compatible = "cy,cyttsp4_i2c_adapter", }, { }
};
MODULE_DEVICE_TABLE(of, cyttsp4_i2c_of_match);

static int cyttsp4_i2c_probe(struct i2c_client *client)
{
	struct device *dev = &client->dev;
	const struct of_device_id *match;
	struct cyttsp4 *ts;

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		dev_err(&client->dev, "I2C functionality not Supported\n");
		return -EIO;
	}

	match = of_match_device(of_match_ptr(cyttsp4_i2c_of_match), dev);
	if (match)
		cyttsp4_devtree_create_and_get_pdata(dev);

	ts = cyttsp4_probe(&cyttsp4_i2c_bus_ops, &client->dev, client->irq,
			  CYTTSP4_I2C_DATA_SIZE);

	if (ts && match)
		cyttsp4_devtree_clean_pdata(dev);

	return PTR_ERR_OR_ZERO(ts);
}

static void cyttsp4_i2c_remove(struct i2c_client *client)
{
	struct device *dev = &client->dev;
	const struct of_device_id *match;
	struct cyttsp4 *ts = i2c_get_clientdata(client);

	cyttsp4_remove(ts);

	match = of_match_device(of_match_ptr(cyttsp4_i2c_of_match), dev);
	if (match)
		cyttsp4_devtree_clean_pdata(dev);
}

static const struct i2c_device_id cyttsp4_i2c_id[] = {
	{ CYTTSP4_I2C_NAME, 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, cyttsp4_i2c_id);

static struct i2c_driver cyttsp4_i2c_driver = {
	.driver = {
		.name	= CYTTSP4_I2C_NAME,
		.pm	= pm_ptr(&cyttsp4_pm_ops),
		.of_match_table = cyttsp4_i2c_of_match,
	},
	.probe		= cyttsp4_i2c_probe,
	.remove		= cyttsp4_i2c_remove,
	.id_table	= cyttsp4_i2c_id,
};

module_i2c_driver(cyttsp4_i2c_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Cypress TrueTouch(R) Standard Product (TTSP) I2C driver");
MODULE_AUTHOR("Cypress");
