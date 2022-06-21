#ifndef __include_study_h__
#define __include_study_h__

#include <linux/cdev.h>
#include <linux/semaphore.h>

#define DRIVER_NAME "study"

#define MINOR_BASE 0
#define MINOR_NUM 2

typedef struct study_device_s study_device_t;
typedef struct study_file_s study_file_t;

extern study_device_t *dev;

int study_close(struct inode *inode, struct file *file);
long study_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
int study_open(struct inode *inode, struct file *file);
ssize_t study_read(struct file *file, char __user *buf, size_t len, loff_t *pos);
ssize_t study_write(struct file *file, const char __user *buf, size_t len, loff_t *pos);

struct study_device_s {
  struct cdev _cdev;
  struct class *_class;
  int _major;
  study_file_t *head;
  struct semaphore sem;
  study_file_t **tail;
};

struct study_file_s {
  loff_t cur;
  study_file_t *next;
  char text[6];
};

#endif /* __include_study_h__ */
