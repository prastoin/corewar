#ifndef _UNISTD_H
#define _UNISTD_H

#include <stdint.h>
#include <stddef.h>

typedef int64_t ssize_t;

ssize_t read(int fd, void *buf, size_t size);
ssize_t write(int fd, void *buf, size_t size);

#define SEEK_CUR 1
#define STDOUT_FILENO 1

#endif
