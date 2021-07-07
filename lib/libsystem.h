#ifndef __LIBSYSTEM_H__
#define __LIBSYSTEM_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int systemos(const char *cmdstring);
int systemosp(const char *cmdstring, char *buf, size_t len);

#endif /* !__LIBSYSTEM_H__ */
