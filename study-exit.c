#include <linux/fs.h>
#include <linux/module.h>

#include "study.h"

static void study_exit(void) {
  for (int i = 0; i < MINOR_NUM; i++) {
    dev_t dev = MKDEV(study_major, MINOR_BASE + i);
    device_destroy(study_class, dev);
  }

  dev_t dev = MKDEV(study_major, MINOR_BASE);
  class_destroy(study_class);
  cdev_del(&study_cdev);
  unregister_chrdev_region(dev, MINOR_NUM);

  printk(KERN_INFO DRIVER_NAME ": Bye World!!\n");
}

module_exit(study_exit);
