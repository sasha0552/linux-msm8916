#ifndef _LINUX_CYTTSP4_DEVTREE_H
#define _LINUX_CYTTSP4_DEVTREE_H

#include <linux/device.h>

#if 1
int cyttsp4_devtree_create_and_get_pdata(struct device *adap_dev);
int cyttsp4_devtree_clean_pdata(struct device *adap_dev);
#else
static inline int cyttsp4_devtree_create_and_get_pdata(struct device *adap_dev)
{
	return 0;
}

static inline int cyttsp4_devtree_clean_pdata(struct device *adap_dev)
{
	return 0;
}
#endif

#endif /* _LINUX_CYTTSP4_DEVTREE_H */
