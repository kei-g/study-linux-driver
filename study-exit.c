#include "study.h"

#include <linux/fs.h>
#include <linux/module.h>

static void study_exit(void)
{
	for (int i = 0; i < MINOR_NUM; i++) {
		dev_t dev = MKDEV(study_major, MINOR_BASE + i);
		device_destroy(study_class, dev);
	}

	dev_t dev = MKDEV(study_major, MINOR_BASE);
	class_destroy(study_class);
	cdev_del(&study_cdev);
	unregister_chrdev_region(dev, MINOR_NUM);

	printk("\x1b[33m%s\x1b[m: Bye World!!\n", DRIVER_NAME);
}

module_exit(study_exit);
