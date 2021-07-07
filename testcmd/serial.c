#include "serial.h"

int usart_test(int argc, char *argv[])
{
    if (argc == 1 || argc > 3) {
        printfs("parameters error. example : %s /dev/ttyUSB0 [times]\n", 
                __STRING_NAME(USART_CMD_NAME));
        return 1;
    }

    int usartfd;
    if ((usartfd = libuart_open(argv[1])) < 0) {
        return 1;
    }

    libuart_init(usartfd, TEST_UART_BAUDRATE, TEST_UART_BITS, 
                          TEST_UART_PARITY, TEST_UART_STOP);

    char rx_buf[USART_TEST_LENGTH];
    int  retval = 1;
    int  times = argc == 3 ? atoi(argv[2]) : TEST_UART_TIMES;
    for (int i = 0; i < times; i++) {
        memset(rx_buf, 0, USART_TEST_LENGTH);
        libuart_send_data(usartfd, USART_TEST_STRING, USART_TEST_LENGTH);
        libuart_read_data(usartfd, rx_buf, USART_TEST_LENGTH, 500);
        printfs("tx_buf(%d) = %s\n", i, USART_TEST_STRING);
        printfs("rx_buf(%d) = %s\n", i, rx_buf);
        if (strcmp(USART_TEST_STRING, rx_buf) == 0) {
            retval = 0; break;
        }
    }

    libuart_close(&usartfd);
    return retval;
}
