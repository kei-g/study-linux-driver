#include <linux/module.h>

static void test_exit(void)
{
	printk("\x1b[33mtest\x1b[m: %s: Bye World!!\n", __FILE__);
}

module_exit(test_exit);
