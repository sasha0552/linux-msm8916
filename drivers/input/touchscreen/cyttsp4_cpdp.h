#ifndef _LINUX_CYTTSP4_CPDP_H
#define _LINUX_CYTTSP4_CPDP_H

#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/platform_data/cyttsp4.h>

struct cyttsp4_platform_data *cyttsp4_dev_get_platdata(struct device *dev);

#endif /* _LINUX_CYTTSP4_CPDP_H */
