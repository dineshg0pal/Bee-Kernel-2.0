#ifndef KLOG_H
#define KLOG_H

/* Kernel log levels */

#define KLOG_INFO  0
#define KLOG_MEM   1
#define KLOG_INT   2
#define KLOG_USER  3
#define KLOG_ERR   4

/* Basic logger (kept for compatibility) */
void klog(const char* msg);

/* Level-based logger */
void klog_level(int level, const char* msg);

#endif
