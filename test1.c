#include "test.h"

#include <linux/fs.h>
#include <linux/module.h>
#include <linux/slab.h>

static int test_close(struct inode *inode, struct file *file)
{
	return 0;
}

static int test_open(struct inode *inode, struct file *file)
{
	return 0;
}

static ssize_t test_read(struct file *file, char __user *buf, size_t len, loff_t *pos)
{
	*buf++ = 't';
	*buf++ = 'e';
	*buf++ = 's';
	*buf++ = 't';
	*buf++ = '\n';
	return 5;
}

static ssize_t test_write(struct file *file, const char __user *buf, size_t len, loff_t *pos)
{
	printk("\x1b[33mtest\x1b[m allocating %zu bytes\n", len + 1);
	char *p = kmalloc(len + 1, GFP_KERNEL);
	printk("\x1b[33mtest\x1b[m allocated in %p\n", p);
	for (size_t i = 0; i < len; i++)
		p[i] = buf[i];
	p[len] = '\0';
	printk("\x1b[33mtest\x1b[m write(%s)\n", p);
	kfree(p);
	return len;
}

struct file_operations fops = {
	.open = test_open,
	.read = test_read,
	.release = test_close,
	.write = test_write,
};

struct cdev testdevice_cdev;
struct class *testdevice_class;
int testdevice_major;

#define DRIVER_NAME "test"

static int test_init(void)
{
	printk("\x1b[33mtest\x1b[m: Hello World!!\n");
	dev_t dev;
	int err = alloc_chrdev_region(&dev, MINOR_BASE, MINOR_NUM, DRIVER_NAME);
	if (err) {
		printk(KERN_ERR "\x1b[31mtest\x1b[m: alloc_chrdev_region failed, %d\n", err);
		return -1;
	}
	testdevice_major = MAJOR(dev);
	dev = MKDEV(testdevice_major, MINOR_BASE);
	cdev_init(&testdevice_cdev, &fops);
	testdevice_cdev.owner = THIS_MODULE;
	err = cdev_add(&testdevice_cdev, dev, MINOR_NUM);
	if (err) {
		printk(KERN_ERR "\x1b[31mtest\x1b[m: cdev_add failed, %d\n", err);
		unregister_chrdev_region(dev, MINOR_NUM);
		return -1;
	}

	testdevice_class = class_create(THIS_MODULE, "test");
	if (IS_ERR(testdevice_class)) {
		printk(KERN_ERR "\x1b[31mtest\x1b[m: failed to class_create\n");
		cdev_del(&testdevice_cdev);
		unregister_chrdev_region(dev, MINOR_NUM);
		return -1;
	}

	for (int i = 0; i < MINOR_NUM; i++) {
		dev = MKDEV(testdevice_major, MINOR_BASE + i);
		device_create(testdevice_class, NULL, dev, NULL, "test%d", MINOR_BASE + i);
	}

	return 0;
}

module_init(test_init);

MODULE_AUTHOR("kei-g");
MODULE_LICENSE("GPL");
