#include <linux/fs.h>
#include <linux/module.h>

#include "study.h"

struct file_operations fops = {
  .compat_ioctl = study_ioctl,
  .open = study_open,
  .read = study_read,
  .release = study_close,
  .unlocked_ioctl = study_ioctl,
  .write = study_write,
};

struct cdev study_cdev;
struct class *study_class;
int study_major;

static int study_init(void) {
  printk(KERN_INFO DRIVER_NAME ": Hello World!!\n");
  dev_t dev;
  int err = alloc_chrdev_region(&dev, MINOR_BASE, MINOR_NUM, DRIVER_NAME);
  if (err) {
    printk(KERN_ERR DRIVER_NAME ": alloc_chrdev_region failed, %d\n", err);
    return -1;
  }
  study_major = MAJOR(dev);
  dev = MKDEV(study_major, MINOR_BASE);
  cdev_init(&study_cdev, &fops);
  study_cdev.owner = THIS_MODULE;
  err = cdev_add(&study_cdev, dev, MINOR_NUM);
  if (err) {
    printk(KERN_ERR DRIVER_NAME ": cdev_add failed, %d\n", err);
    unregister_chrdev_region(dev, MINOR_NUM);
    return -1;
  }

  study_class = class_create(THIS_MODULE, "study");
  if (IS_ERR(study_class)) {
    printk(KERN_ERR DRIVER_NAME ": failed to class_create\n");
    cdev_del(&study_cdev);
    unregister_chrdev_region(dev, MINOR_NUM);
    return -1;
  }

  for (int i = 0; i < MINOR_NUM; i++) {
    dev = MKDEV(study_major, MINOR_BASE + i);
    device_create(study_class, NULL, dev, NULL, DRIVER_NAME "%d", MINOR_BASE + i);
  }

  return 0;
}

module_init(study_init);

MODULE_AUTHOR("kei-g");
MODULE_LICENSE("GPL");
