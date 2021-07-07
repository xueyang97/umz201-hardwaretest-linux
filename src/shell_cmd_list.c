/**
 * @file shell_cmd_list.c
 * @author Letter (NevermindZZT@gmail.com)
 * @brief shell cmd list
 * @version 3.0.0
 * @date 2020-01-17
 * 
 * @copyright (c) 2020 Letter
 * 
 */

#include "shell.h"
#include "shell_ext.h"

#if SHELL_USING_CMD_EXPORT != 1

extern int shellSetVar(char *name, int value);
extern void shellUp(Shell *shell);
extern void shellDown(Shell *shell);
extern void shellRight(Shell *shell);
extern void shellLeft(Shell *shell);
extern void shellTab(Shell *shell);
extern void shellBackspace(Shell *shell);
extern void shellDelete(Shell *shell);
extern void shellEnter(Shell *shell);
extern void shellHelp(int argc, char *argv[]);
extern void shellUsers(void);
extern void shellCmds(void);
extern void shellVars(void);
extern void shellKeys(void);
extern void shellClear(void);
extern void shellExit(void);


#include "../cmd/hello.h"
#include "../testcmd/test.h"
#include "../qmi/modemtool.h"

/**
 * @brief shell命令表
 * 
 */
const ShellCommand shellCommandList[] = 
{
    {.attr.value=SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_USER),
    .data.user.name = SHELL_DEFAULT_USER,
    .data.user.password = SHELL_DEFAULT_USER_PASSWORD,
    .data.user.desc = "defalut user"},
    SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC),
                   setVar, shellSetVar, set var),
    SHELL_KEY_ITEM(SHELL_CMD_PERMISSION(0), SHELL_KEY_VALUE_UP, shellUp, up),
    SHELL_KEY_ITEM(SHELL_CMD_PERMISSION(0), SHELL_KEY_VALUE_DOWN, shellDown, down),
    SHELL_KEY_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_ENABLE_UNCHECKED,
                   SHELL_KEY_VALUE_RIGTH, shellRight, right),
    SHELL_KEY_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_ENABLE_UNCHECKED,
                   SHELL_KEY_VALUE_LEFT, shellLeft, left),
    SHELL_KEY_ITEM(SHELL_CMD_PERMISSION(0), SHELL_KEY_VALUE_TAB, shellTab, tab),
    SHELL_KEY_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_ENABLE_UNCHECKED,
                   SHELL_KEY_VALUE_BACK, shellBackspace, backspace),
    SHELL_KEY_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_ENABLE_UNCHECKED,
                   SHELL_KEY_VALUE_DELETE, shellDelete, delete),
    // SHELL_KEY_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_ENABLE_UNCHECKED,
    //                0x1B5B337E, shellDelete, delete),
#if SHELL_ENTER_LF == 1
    SHELL_KEY_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_ENABLE_UNCHECKED,
                   SHELL_KEY_VALUE_LF, shellEnter, enter),
#endif
#if SHELL_ENTER_CR == 1
    SHELL_KEY_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_ENABLE_UNCHECKED,
                   SHELL_KEY_VALUE_CR, shellEnter, enter),
#endif
#if SHELL_ENTER_CRLF == 1
    SHELL_KEY_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_ENABLE_UNCHECKED,
                   SHELL_KEY_VALUE_CRLF, shellEnter, enter),
#endif
    SHELL_CMD_ITEM(
                   SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
                   help, shellHelp, show command info\nhelp [cmd]),
    SHELL_CMD_ITEM(
                   SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC)|SHELL_CMD_DISABLE_RETURN,
                   users, shellUsers, list all user),
    SHELL_CMD_ITEM(
                   SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC)|SHELL_CMD_DISABLE_RETURN,
                   cmds, shellCmds, list all cmd),
    SHELL_CMD_ITEM(
                   SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC)|SHELL_CMD_DISABLE_RETURN,
                   vars, shellVars, list all var),
    SHELL_CMD_ITEM(
                   SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC)|SHELL_CMD_DISABLE_RETURN,
                   keys, shellKeys, list all key),
    SHELL_CMD_ITEM(
                   SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC)|SHELL_CMD_DISABLE_RETURN,
                   clear, shellClear, clear console),
    SHELL_CMD_ITEM(
                   SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC)|SHELL_CMD_DISABLE_RETURN,
                   exit, shellExit, exit shell),

    /* 用户cmd命令, 建议使用 SHELL_TYPE_CMD_MAIN 模式, 
       SHELL_TYPE_CMD_FUNC 模式在移植过程中CUP位数问题未处理完成,会产生bug */
    SHELL_CMD_ITEM(
                   SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN)|SHELL_CMD_DISABLE_RETURN,
                   hello, hello, Hello world Test CMD),
    SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN),
                   testuart, usart_test, test serial port tty),
    SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN),
                   testspeaker, test_speaker, test speaker),
    SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN),
                   testusb, test_usbnum, test usb number),
    SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN),
                   testimei, qmi_test_imei, test LTE imei),
    SHELL_CMD_ITEM(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN),
                   testlte, qmi_test_lte, test LTE mode and signal),
};


/**
 * @brief shell命令表大小
 * 
 */
const unsigned short shellCommandCount 
    = sizeof(shellCommandList) / sizeof(ShellCommand);

#endif
