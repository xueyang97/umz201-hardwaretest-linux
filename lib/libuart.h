#ifndef __LIBUART_H__
#define __LIBUART_H__

#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>  
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <termios.h>
#include <errno.h>   
#include <limits.h> 
#include <asm/ioctls.h>
#include <time.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <sys/epoll.h> 
#include <string.h>

#include "libiom.h"

#define USART_STOP_1        1
#define USART_STOP_2        2

#define USART_DEFAULT_BAUDRATE          B115200
#define USART_DEFAULT_BAUDRATE_NAME     "115200"
#define USART_DEFAULT_BITS              CS8
#define USART_DEFAULT_BITS_NAME         "bit8"
#define USART_DEFAULT_PARITY            'N'
#define USART_DEFAULT_PARITY_NAME       "none parity"
#define USART_DEFAULT_STOP              USART_STOP_1
#define USART_DEFAULT_STOP_NAME         "stop bit 1"


int libuart_open(const char *ttyn);
int libuart_close(int *fd);
int libuart_init(int fd, int baudrate, int bits, int parity, int stop);
int libuart_read_data(int fd, void *buffer, int length, int timeout);
int libuart_send_data(int fd, const void *buffer, size_t length);


#endif /* __LIBUART_H__ */
