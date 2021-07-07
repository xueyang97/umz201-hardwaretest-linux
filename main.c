#include "./src/shell.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>

Shell shell;
char shellBuffer[1024];

#if SHELL_TERMINAL_IO_MODE == 0

int terminal_init(void *arg)
{
    return 0;
}

/**
 * @brief 用户shell写
 * 
 * @param data 数据
 */
void userShellWrite(char data)
{
    putchar(data);
}


/**
 * @brief 用户shell读
 * 
 * @param data 数据
 * @return char 状态
 */
signed char userShellRead(char *data)
{
    *data = getchar();
    return 0;
}

#elif SHELL_TERMINAL_IO_MODE == 1

#include "libuart.h"

int uartfd = -1;

int terminal_init(void *arg)
{
    if (arg == NULL)    return -1;

    if ((uartfd = libuart_open((char *)arg)) < 0) {
        return 1;
    }

    return libuart_init(uartfd, TERMINAL_UART_BAUDRATE, TERMINAL_UART_BITS, 
                         TERMINAL_UART_PARITY, TERMINAL_UART_STOP);
}

void userShellWrite(char data)
{
    libuart_send_data(uartfd, &data, 1);
}

signed char userShellRead(char *data)
{
    int bytes;
    ioctl(uartfd, FIONREAD, &bytes);
    if (bytes > 0) {
        read(uartfd, data, 1);
        return 0;
    } else {
        return -1;
    }
}

#else
 #error "shell terminal I/O mode error!"
#endif

/**
 * @brief 用户shell初始化
 * 
 */
void userShellInit(int argc)
{
    terminal_init(TERMINAL_PORT);
    shell.write = userShellWrite;
    shell.read = userShellRead;
    shellInit(&shell, shellBuffer, sizeof(shellBuffer), USE_ONE_CMD_MODE(argc));
}

#define list_entry(ptr, type, member) ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

int main(int argc, char *argv[])
{
    userShellInit(argc);
    return shellTask(&shell, argc, argv);
}

