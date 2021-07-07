#ifndef __TEST_H_
#define __TEST_H_

#include "../lib/libiom.h"
#include "../lib/libuart.h"
#include "../lib/libsystem.h"
#include "../lib/libstring.h"

#include "serial.h"
#define USART_CMD_NAME      testuart
#define USART_TEST_STRING   "testuart"
#define USART_TEST_LENGTH   sizeof(USART_TEST_STRING)
#define USART_TEST_TIMEOUT  500
#define TEST_UART_TIMES     3

#define TEST_UART_BAUDRATE  115200
#define TEST_UART_BITS      8
#define TEST_UART_PARITY    'N'
#define TEST_UART_STOP      1

#include "play.h"
#define PLAY_CMD_NAME      testspeaker

#include "usbnum.h"
#define USBNUB_CMD_NAME    testusb


#endif /* !__TEST_H_ */
