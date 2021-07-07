#include "hello.h"

int hello(int argc, char *argv[])
{
    for (int i = 0; i < argc; i++) {
        printfs("param[%d] = %s\n", i, argv[i]);
    }
    return 0;
}
