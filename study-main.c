#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/slab.h>

#include "study.h"

int study_close(struct inode *inode, struct file *file) {
  study_device_t *dev = container_of(inode->i_cdev, study_device_t, _cdev);
  if (down_interruptible(&dev->sem))
    return -ERESTARTSYS;
  study_file_t *fp = file->private_data;
  if (fp == dev->head)
    dev->head = fp->next;
  else
    for (study_file_t *node = dev->head; node->next; node = node->next)
      if (node->next == fp) {
        node->next = fp->next;
        break;
      }
  if (!dev->head)
    dev->tail = &dev->head;
  up(&dev->sem);
  kfree(fp);
  return 0;
}

long study_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
  return 0;
}

int study_open(struct inode *inode, struct file *file) {
  study_file_t *fp = kmalloc(sizeof(*fp), GFP_KERNEL);
  fp->cur = 0;
  fp->next = NULL;
  strcpy(fp->text, "test\n");
  file->private_data = fp;
  study_device_t *dev = container_of(inode->i_cdev, study_device_t, _cdev);
  if (down_interruptible(&dev->sem)) {
    file->private_data = NULL;
    kfree(fp);
    return -ERESTARTSYS;
  }
  *dev->tail = fp;
  dev->tail = &fp->next;
  up(&dev->sem);
  return 0;
}

unsigned int study_poll(struct file *file, poll_table *pt) {
  study_file_t *fp = file->private_data;
  unsigned int mask = POLLIN | POLLRDNORM;
  mask |= POLLOUT | POLLWRNORM;
  if (sizeof(fp->text) <= fp->cur)
    mask |= POLLHUP;
  return mask;
}

ssize_t study_read(struct file *file, char __user *buf, size_t len, loff_t *pos) {
  study_file_t *fp = file->private_data;
  printk(KERN_INFO DRIVER_NAME ": read is called, len=%zu, pos=%lld, file->cur=%lld\n",
         len, *pos, fp->cur);
  ssize_t rlen = sizeof(fp->text) - fp->cur;
  if (len < rlen)
    rlen = len;
  if (raw_copy_to_user(buf, fp->text + fp->cur, rlen))
    return -EFAULT;
  fp->cur += rlen;
  return rlen;
}

ssize_t study_write(struct file *file, const char __user *buf, size_t len, loff_t *pos) {
  printk(KERN_INFO DRIVER_NAME ": write is called, len=%zu, pos=%lld\n", len, *pos);
  printk(KERN_INFO DRIVER_NAME ": allocating %zu bytes\n", len + 1);
  char *p = kmalloc(len + 1, GFP_KERNEL);
  printk(KERN_INFO DRIVER_NAME ": allocated in %p\n", p);
  if (raw_copy_from_user(p, buf, len))
    return -EFAULT;
  p[len] = '\0';
  printk(KERN_INFO DRIVER_NAME ": write(%s)\n", p);
  kfree(p);
  return len;
}
