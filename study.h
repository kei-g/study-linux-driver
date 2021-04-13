#ifndef __include_study_h__
#define __include_study_h__

#include <linux/cdev.h>

#define DRIVER_NAME "study"

#define MINOR_BASE 0
#define MINOR_NUM 2

extern struct cdev study_cdev;
extern struct class *study_class;
extern int study_major;

#endif /* __include_study_h__ */
