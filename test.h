#ifndef __include_test_h__
#define __include_test_h__

#include <linux/cdev.h>

#define MINOR_BASE 0
#define MINOR_NUM 2

extern struct cdev testdevice_cdev;
extern struct class *testdevice_class;
extern int testdevice_major;

#endif /* __include_test_h__ */
