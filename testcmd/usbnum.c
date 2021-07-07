#include "usbnum.h"

int test_usbnum(int argc, char *argv[])
{
    if (argc != 2) {
        printfs("parameters error. example : %s num(4)\n", 
                __STRING_NAME(USBNUB_CMD_NAME));
        return 1;
    }
    char buffer[1024*2];
    int usbnum = 0, retval = 0;
    if ((retval = systemosp("lsusb",buffer,sizeof(buffer))) == 0) {
        usbnum = findstr(buffer, "Device");
    }
    printfs("%s\n",buffer);
    printfs("usbnum = %d\n",usbnum);
    
    if (atoi(argv[1]) != usbnum) {
        return 2;
    }
    return retval;
}

