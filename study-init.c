#include <linux/fs.h>
#include <linux/module.h>
#include <linux/slab.h>

#include "study.h"

struct file_operations fops = {
  .compat_ioctl = study_ioctl,
  .open = study_open,
  .owner = THIS_MODULE,
  .read = study_read,
  .release = study_close,
  .unlocked_ioctl = study_ioctl,
  .write = study_write,
};

study_device_t *dev = NULL;

static int study_init(void) {
  printk(KERN_INFO DRIVER_NAME ": Hello World!!\n");
  dev = kmalloc(sizeof(*dev), GFP_KERNEL);
  dev_t dno;
  int err = alloc_chrdev_region(&dno, MINOR_BASE, MINOR_NUM, DRIVER_NAME);
  if (err) {
    printk(KERN_ERR DRIVER_NAME ": alloc_chrdev_region failed, %d\n", err);
    kfree(dev);
    return -1;
  }
  dev->_major = MAJOR(dno);
  dno = MKDEV(dev->_major, MINOR_BASE);
  cdev_init(&dev->_cdev, &fops);
  dev->_cdev.owner = THIS_MODULE;
  err = cdev_add(&dev->_cdev, dno, MINOR_NUM);
  if (err) {
    printk(KERN_ERR DRIVER_NAME ": cdev_add failed, %d\n", err);
    unregister_chrdev_region(dno, MINOR_NUM);
    kfree(dev);
    return -1;
  }

  dev->_class = class_create(THIS_MODULE, DRIVER_NAME);
  if (IS_ERR(dev->_class)) {
    printk(KERN_ERR DRIVER_NAME ": failed to class_create\n");
    cdev_del(&dev->_cdev);
    unregister_chrdev_region(dno, MINOR_NUM);
    kfree(dev);
    return -1;
  }

  dev->head = NULL;
  sema_init(&dev->sem, 1);
  dev->tail = &dev->head;

  for (int i = 0; i < MINOR_NUM; i++) {
    dno = MKDEV(dev->_major, MINOR_BASE + i);
    device_create(dev->_class, NULL, dno, NULL, DRIVER_NAME "%d", MINOR_BASE + i);
  }

  return 0;
}

module_init(study_init);

MODULE_AUTHOR("kei-g");
MODULE_LICENSE("GPL");
