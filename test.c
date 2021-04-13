#include <linux/module.h>

static int test_init(void)
{
	printk("Hello World!!\n");
	return 0;
}

static void test_exit(void)
{
	printk("Bye World!!\n");
}

module_init(test_init);
module_exit(test_exit);

MODULE_AUTHOR("kei-g");
MODULE_LICENSE("GPL");
