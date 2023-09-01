#include "cyttp4_cpdp.h"

static int cy_hw_power(struct cyttsp4_core_platform_data *pdata, int on, int use_irq)
{
	int ret = 0;

	pr_info("[TSP] %s : power %s\n", CYTTSP4_I2C_NAME, on ? "on" : "off");

	if (use_irq) {
		if (on) {
			//enable_irq(gpio_to_irq(irq_gpio));
			//pr_debug("Enabled IRQ %d for TSP\n", gpio_to_irq(irq_gpio));
		} else {
			//pr_debug("Disabling IRQ %d for TSP\n", gpio_to_irq(irq_gpio));
			//disable_irq(gpio_to_irq(irq_gpio));
		}
	}

	ret = gpio_direction_output(pdata->vddo_gpio, on);
	if (ret) {
		pr_err("[TSP] %s : %s: unable to set_direction for gpio[%d] %d\n",
			 CYTTSP4_I2C_NAME, __func__, pdata->vddo_gpio, ret);
		return -EINVAL;
	}
//	msleep(50);

	ret = gpio_direction_output(pdata->avdd_gpio, on);
	if (ret) {
		pr_err("[TSP] %s : %s: unable to set_direction for gpio[%d] %d\n",
			 CYTTSP4_I2C_NAME, __func__, pdata->avdd_gpio, ret);
		return -EINVAL;
	}
	msleep(50);
	return 0;
}

static int cyttsp4_xres(struct cyttsp4_core_platform_data *pdata,
		struct device *dev)
{
	int irq_gpio = pdata->irq_gpio;

	int rc = 0;
	printk(" The TOUCH  IRQ no in cyttsp4_xres() is %d",irq_gpio );
	cy_hw_power(pdata, 0, true);

	cy_hw_power(pdata, 1, true);

	return rc;
}

static int cyttsp4_init(struct cyttsp4_core_platform_data *pdata,
		int on, struct device *dev)
{
	int rc = 0;
	int irq_gpio = pdata->irq_gpio;

	if (on) {
		rc = gpio_request(irq_gpio, "TSP_INT");
		if(rc < 0){
			dev_err(dev, "%s: unable to request TSP_INT\n", __func__);
			return rc;
		}
		gpio_direction_input(irq_gpio);
		rc = gpio_request(pdata->avdd_gpio, "TSP_AVDD_gpio");
		if(rc < 0){
			dev_err(dev, "%s: unable to request TSP_AVDD_gpio\n", __func__);
			return rc;
		}
		rc = gpio_request(pdata->vddo_gpio, "TSP_VDDO_gpio");
		if(rc < 0){
			dev_err(dev, "%s: unable to request TSP_VDDO_gpio\n", __func__);
			return rc;
		}

		cy_hw_power(pdata, 1, false);
	} else {
		cy_hw_power(pdata, 0, false);
		gpio_free(irq_gpio);
	}
	dev_info(dev,
		"%s: INIT CYTTSP IRQ gpio=%d onoff=%d r=%d\n",
		__func__, irq_gpio, on, rc);
	return rc;
}

static int cyttsp4_wakeup(struct cyttsp4_core_platform_data *pdata,
		struct device *dev, atomic_t *ignore_irq)
{
	return cy_hw_power(pdata, 1, true);
}

static int cyttsp4_sleep(struct cyttsp4_core_platform_data *pdata,
		struct device *dev, atomic_t *ignore_irq)
{
	return cy_hw_power(pdata, 0, true);
}

static int cyttsp4_power(struct cyttsp4_core_platform_data *pdata,
		int on, struct device *dev, atomic_t *ignore_irq)
{
	if (on)
		return cyttsp4_wakeup(pdata, dev, ignore_irq);

	return cyttsp4_sleep(pdata, dev, ignore_irq);
}

static int cyttsp4_irq_stat(struct cyttsp4_core_platform_data *pdata,
		struct device *dev)
{
	return gpio_get_value(pdata->irq_gpio);
}

struct cyttsp4_platform_data *cyttsp4_dev_get_platdata(struct device *dev)
{
	struct cyttsp4_platform_data *pdata;
	struct cyttsp4_core_platform_data *core_pdata;
	struct cyttsp4_mt_platform_data *mt_pdata;
	struct touch_framework *frmwrk;

	frmwrk = kzalloc(sizeof(*frmwrk), GFP_KERNEL);
	frmwrk->abs = {53, 0, 540, 0, 0, 54, 0, 960, 0, 0, 58, 0, 255, 0, 0, 65535, 0, 255, 0, 0, 57, 0, 15, 0, 0, 48, 0, 255, 0, 0, 49, 0, 255, 0, 0, 52, 65239, 127, 0, 0};
	frmwrk->size = 40;
	frmwrk->enable_vkeys = 0;

	core_pdata = kzalloc(sizeof(*core_pdata), GFP_KERNEL);
	core_pdata->irq_gpio = 915;
	core_pdata->rst_gpio = 0;
	core_pdata->level_irq_udelay = 0;
	core_pdata->xres = cyttsp4_xres;
	core_pdata->init = cyttsp4_init;
	core_pdata->power = cyttsp4_power;
	core_pdata->irq_stat = cyttsp4_irq_stat;
	core_pdata->sett = NULL;
	core_pdata->avdd_gpio = 975;
	core_pdata->vddo_gpio = 1000;

	mt_pdata = kzalloc(sizeof(*mt_pdata), GFP_KERNEL);
	mt_pdata->frmwrk = frmwrk;
	mt_pdata->flags = 0;
	mt_pdata->inp_dev_name = "sec_touchscreen";

	pdata = kzalloc(sizeof(*pdata), GFP_KERNEL);
	pdata->core_pdata = core_pdata;
	pdata->mt_pdata = mt_pdata;

	return pdata;
}
EXPORT_SYMBOL_GPL(cyttsp4_dev_get_platdata);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("CYTTSP4 Constant Platform Data Provider");
MODULE_AUTHOR("sasha0552");
