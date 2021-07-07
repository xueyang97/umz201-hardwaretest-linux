#include "libiom.h"

int printfs(const char *fmt, ...)
{
    Shell *shell = shellGetCurrent();
    SHELL_ASSERT(shell, return 0);

    char buffer[SHELL_PRINT_BUFFER];
    va_list vargs;

    va_start(vargs, fmt);
    vsnprintf(buffer, SHELL_PRINT_BUFFER - 1, fmt, vargs);
    va_end(vargs);
    
    return (int)shellWriteString(shell, buffer);
}

int vsprintfs(char *buffer, const char *fmt, ...)
{
    va_list vargs;
    va_start(vargs, fmt);
    return vsprintf(buffer, fmt, vargs);
}

int systems(char *cmdstring)
{
    return 0;
}

