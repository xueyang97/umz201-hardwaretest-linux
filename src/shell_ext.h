/**
 * @file shell_ext.h
 * @author Letter (NevermindZZT@gmail.com)
 * @brief shell extensions
 * @version 3.0.0
 * @date 2019-12-31
 * 
 * @Copyright (c) 2019 Letter
 * 
 */

#ifndef __SHELL_EXT_H__
#define __SHELL_EXT_H__

#include "shell.h"

/* 定义键盘按键的键值 大端模式 */
#define SHELL_KEY_VALUE_UP      0x1b5b4100
#define SHELL_KEY_VALUE_DOWN    0x1b5b4200
#define SHELL_KEY_VALUE_RIGTH   0x1b5b4300
#define SHELL_KEY_VALUE_LEFT    0x1b5b4400
#define SHELL_KEY_VALUE_TAB     0x09000000
#define SHELL_KEY_VALUE_BACK    0x7f000000
#define SHELL_KEY_VALUE_DELETE  0x1B5B337E
#define SHELL_KEY_VALUE_LF      0x0a000000
#define SHELL_KEY_VALUE_CR      0x0b000000
#define SHELL_KEY_VALUE_CRLF    0x0b0a0000

/**
 * @brief 数字类型
 * 
 */
typedef enum
{
    NUM_TYPE_INT,                                           /**< 十进制整型 */
    NUM_TYPE_BIN,                                           /**< 二进制整型 */
    NUM_TYPE_OCT,                                           /**< 八进制整型 */
    NUM_TYPE_HEX,                                           /**< 十六进制整型 */
    NUM_TYPE_FLOAT                                          /**< 浮点型 */
} NUM_Type;

unsigned int shellExtParsePara(Shell *shell, char *string);
int shellExtRun(Shell *shell, ShellCommand *command, int argc, char *argv[]);

#endif
