#include "study.h"

#include <linux/fs.h>
#include <linux/module.h>

struct file_operations fops = {
	.open = study_open,
	.read = study_read,
	.release = study_close,
	.write = study_write,
};

struct cdev study_cdev;
struct class *study_class;
int study_major;

static int study_init(void)
{
	printk("\x1b[33m%s\x1b[m: Hello World!!\n", DRIVER_NAME);
	dev_t dev;
	int err = alloc_chrdev_region(&dev, MINOR_BASE, MINOR_NUM, DRIVER_NAME);
	if (err) {
		printk(KERN_ERR "\x1b[31m%s\x1b[m: alloc_chrdev_region failed, %d\n", DRIVER_NAME, err);
		return -1;
	}
	study_major = MAJOR(dev);
	dev = MKDEV(study_major, MINOR_BASE);
	cdev_init(&study_cdev, &fops);
	study_cdev.owner = THIS_MODULE;
	err = cdev_add(&study_cdev, dev, MINOR_NUM);
	if (err) {
		printk(KERN_ERR "\x1b[31m%s\x1b[m: cdev_add failed, %d\n", DRIVER_NAME, err);
		unregister_chrdev_region(dev, MINOR_NUM);
		return -1;
	}

	study_class = class_create(THIS_MODULE, "study");
	if (IS_ERR(study_class)) {
		printk(KERN_ERR "\x1b[31m%s\x1b[m: failed to class_create\n", DRIVER_NAME);
		cdev_del(&study_cdev);
		unregister_chrdev_region(dev, MINOR_NUM);
		return -1;
	}

	for (int i = 0; i < MINOR_NUM; i++) {
		dev = MKDEV(study_major, MINOR_BASE + i);
		device_create(study_class, NULL, dev, NULL, "%s%d", DRIVER_NAME, MINOR_BASE + i);
	}

	return 0;
}

module_init(study_init);

MODULE_AUTHOR("kei-g");
MODULE_LICENSE("GPL");
