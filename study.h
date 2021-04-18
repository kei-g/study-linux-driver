#ifndef __include_study_h__
#define __include_study_h__

#include <linux/cdev.h>

#define DRIVER_NAME "study"

#define MINOR_BASE 0
#define MINOR_NUM 2

extern struct cdev study_cdev;
extern struct class *study_class;
extern int study_major;

int study_close(struct inode *inode, struct file *file);
long study_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
int study_open(struct inode *inode, struct file *file);
ssize_t study_read(struct file *file, char __user *buf, size_t len, loff_t *pos);
ssize_t study_write(struct file *file, const char __user *buf, size_t len, loff_t *pos);

#endif /* __include_study_h__ */
