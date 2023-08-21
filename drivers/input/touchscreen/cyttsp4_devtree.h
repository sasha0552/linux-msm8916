#ifdef CONFIG_TOUCHSCREEN_CYTTSP4_DEVTREE
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
