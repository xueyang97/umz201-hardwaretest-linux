#ifndef __SERIAL_H_
#define __SERIAL_H_

#include "../lib/libiom.h"
#include "../lib/libuart.h"
#include "test.h"

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

// #define USART_STOP_1        1
// #define USART_STOP_2        2

// #define USART_DEFAULT_BAUDRATE          B115200
// #define USART_DEFAULT_BAUDRATE_NAME     "115200"
// #define USART_DEFAULT_BITS              CS8
// #define USART_DEFAULT_BITS_NAME         "bit8"
// #define USART_DEFAULT_PARITY            'N'
// #define USART_DEFAULT_PARITY_NAME       "none parity"
// #define USART_DEFAULT_STOP              USART_STOP_1
// #define USART_DEFAULT_STOP_NAME         "stop bit 1"


extern int usart_test(int argc, char *argv[]);

#endif /* !__SERIAL_H_ */
