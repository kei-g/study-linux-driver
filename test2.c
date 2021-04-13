#include "test.h"

#include <linux/fs.h>
#include <linux/module.h>

static void test_exit(void)
{
	for (int i = 0; i < MINOR_NUM; i++) {
		dev_t dev = MKDEV(testdevice_major, MINOR_BASE + i);
		device_destroy(testdevice_class, dev);
	}

	dev_t dev = MKDEV(testdevice_major, MINOR_BASE);
	class_destroy(testdevide_class);
	cdev_del(&testdevice_cdev);
	unregister_chrdev_region(dev, MINOR_NUM);

	printk("\x1b[33mtest\x1b[m: Bye World!!\n");
}

module_exit(test_exit);
