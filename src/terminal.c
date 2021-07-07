#include "terminal.h"

int printfColor(Shell *shell, char *color, const char *fmt, ...) 
{
    SHELL_ASSERT(shell, return 0);

    char buffer[SHELL_PRINT_BUFFER];
    // int buffer_length = 0;

    shellWriteString(shell, color);

    va_list vargs;
    va_start(vargs, fmt);
    vsnprintf(buffer, SHELL_PRINT_BUFFER - 1, fmt, vargs);
    va_end(vargs);
    int length = shellWriteString(shell, buffer);

    shellWriteString(shell, NONE);

    return length;
}

int terminal_echo_enable(int fd, int option)
{
    int err;
    struct termios term;
    if(tcgetattr(fd,&term) == -1) {
        perror("Cannot get the attribution of the terminal");
        return 1;
    }
    if(option) {
        term.c_lflag |= (ECHO | ICANON);
    } else {
        term.c_lflag &= ~(ECHO | ICANON);
    }

    err = tcsetattr(fd, TCSAFLUSH, &term);
    if(err == -1 && err == EINTR) {
        perror("Cannot set the attribution of the terminal");
        return 1;
    }
    return 0;
}


