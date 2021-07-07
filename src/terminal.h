#ifndef __TERMINAL__
#define __TERMINAL__

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>

#include "shell.h"

#define NONE                 "\e[0m"
#define BLACK                "\e[0;30m"
#define L_BLACK              "\e[1;30m"
#define RED                  "\e[0;31m"
#define L_RED                "\e[1;31m"
#define GREEN                "\e[0;32m"
#define L_GREEN              "\e[1;32m"
#define BROWN                "\e[0;33m"
#define YELLOW               "\e[1;33m"
#define BLUE                 "\e[0;34m"
#define L_BLUE               "\e[1;34m"
#define PURPLE               "\e[0;35m"
#define L_PURPLE             "\e[1;35m"
#define CYAN                 "\e[0;36m"
#define L_CYAN               "\e[1;36m"
#define GRAY                 "\e[0;37m"
#define WHITE                "\e[1;37m"
 
#define BOLD                 "\e[1m"
#define UNDERLINE            "\e[4m"
#define BLINK                "\e[5m"
#define REVERSE              "\e[7m"
#define HIDE                 "\e[8m"
#define CLEAR                "\e[2J"
#define CLRLINE              "\e[1K\r"

int printfColor(Shell *shell, char *color, const char *fmt, ...) ;
int terminal_echo_enable(int fd, int option);

#define printfRed(shell, fmt, ...)          printfColor(shell, RED, fmt, ##__VA_ARGS__)
#define printfLred(shell, fmt, ...)         printfColor(shell, L_RED, fmt, ##__VA_ARGS__)
#define printfGreen(shell, fmt, ...)        printfColor(shell, GREEN, fmt, ##__VA_ARGS__)
#define printfLgreen(shell, fmt, ...)       printfColor(shell, L_GREEN, fmt, ##__VA_ARGS__)


#endif /* !__TERMINAL__ */