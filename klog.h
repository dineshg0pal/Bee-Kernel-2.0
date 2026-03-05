#ifndef KLOG_H
#define KLOG_H

/* Simple kernel logger */

void klog(const char* msg);

/* Print stored kernel logs (for dmsg syscall) */
void klog_dump();

#endif
