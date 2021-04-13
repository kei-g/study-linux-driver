#include "study.h"

#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/slab.h>

typedef struct {
	loff_t cur;
	char text[6];
} study_t;

int study_close(struct inode *inode, struct file *file)
{
	kfree(file->private_data);
	return 0;
}

int study_open(struct inode *inode, struct file *file)
{
	study_t *s = kmalloc(sizeof(*s), GFP_KERNEL);
	s->cur = 0;
	strcpy(s->text, "test\n");
	file->private_data = s;
	return 0;
}

ssize_t study_read(struct file *file, char __user *buf, size_t len, loff_t *pos)
{
	printk("\x1b[33m%s\x1b[m read is called, len=%zu, pos=%lld\n", DRIVER_NAME, len, *pos);
	study_t *s = file->private_data;
	ssize_t rlen = sizeof(s->text) - s->cur;
	if (len < rlen)
		rlen = len;
	if (raw_copy_to_user(buf, s->text + s->cur, rlen))
		return -EFAULT;
	s->cur += rlen;
	return rlen;
}

ssize_t study_write(struct file *file, const char __user *buf, size_t len, loff_t *pos)
{
	printk("\x1b[33m%s\x1b[m write is called, len=%zu, pos=%lld\n", DRIVER_NAME, len, *pos);
	printk("\x1b[33m%s\x1b[m allocating %zu bytes\n", DRIVER_NAME, len + 1);
	char *p = kmalloc(len + 1, GFP_KERNEL);
	printk("\x1b[33m%s\x1b[m allocated in %p\n", DRIVER_NAME, p);
	raw_copy_from_user(p, buf, len);
	p[len] = '\0';
	printk("\x1b[33m%s\x1b[m write(%s)\n", DRIVER_NAME, p);
	kfree(p);
	return len;
}
