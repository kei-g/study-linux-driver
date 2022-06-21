#include <linux/fs.h>
#include <linux/module.h>
#include <linux/slab.h>

#include "study.h"

static void study_exit(void) {
  for (int i = 0; i < MINOR_NUM; i++) {
    dev_t dno = MKDEV(dev->_major, MINOR_BASE + i);
    device_destroy(dev->_class, dno);
  }

  dev_t dno = MKDEV(dev->_major, MINOR_BASE);
  class_destroy(dev->_class);
  cdev_del(&dev->_cdev);
  unregister_chrdev_region(dno, MINOR_NUM);

  for (study_file_t *fp = dev->head; fp;) {
    study_file_t *next = fp->next;
    kfree(fp);
    fp = next;
  }
  kfree(dev);
  dev = NULL;

  printk(KERN_INFO DRIVER_NAME ": Bye World!!\n");
}

module_exit(study_exit);
