#include <linux/module.h>

static int test_init(void)
{
	printk("\x1b[33mtest\x1b[m: Hello World!!\n");
	return 0;
}

module_init(test_init);

MODULE_AUTHOR("kei-g");
MODULE_LICENSE("GPL");
