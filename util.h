#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

ssize_t send_n(int fd, const void *buf, size_t len);

ssize_t recv_n(int fd, void *buf, size_t len);

#endif
