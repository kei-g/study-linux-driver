#include <linux/cdev.h>
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

#define DRIVER_MAJOR 63
#define DRIVER_NAME "test"

static int test_init(void)
{
	printk("\x1b[33mtest\x1b[m: Hello World!!\n");
	register_chrdev(DRIVER_MAJOR, DRIVER_NAME, &fops);
	return 0;
}

module_init(test_init);

MODULE_AUTHOR("kei-g");
MODULE_LICENSE("GPL");
