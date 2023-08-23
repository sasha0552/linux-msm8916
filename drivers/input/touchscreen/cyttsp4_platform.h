#ifndef _LINUX_CYTTSP4_PLATFORM_H
#define _LINUX_CYTTSP4_PLATFORM_H

#include <linux/device.h>

int cyttsp4_xres(struct cyttsp4_core_platform_data *pdata,
		struct device *dev);
int cyttsp4_init(struct cyttsp4_core_platform_data *pdata,
		int on, struct device *dev);
int cyttsp4_power(struct cyttsp4_core_platform_data *pdata,
		int on, struct device *dev, atomic_t *ignore_irq);
int cyttsp4_irq_stat(struct cyttsp4_core_platform_data *pdata,
		struct device *dev);

#endif /* _LINUX_CYTTSP4_PLATFORM_H */
