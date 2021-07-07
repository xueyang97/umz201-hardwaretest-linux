#include "libuart.h"

static void __libuart_set_baudrate(int fd, struct termios *operate, int baudrate);
static void __libuart_set_termios_bits(struct termios *operate, int bits);
static void __libuart_set_termios_parity(struct termios *operate, int parity);
static void __libuart_set_termios_stop(struct termios *operate, int stop);

int libuart_open(const char *ttyn)
{
    // usart_permission(ttyn, "777");
    /* 以非阻塞的模式打开串口 */
    int serialfd = open(ttyn, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (serialfd < 0) {
        printfs("serial port %s open error!\n", ttyn);
    }
    
    return serialfd;
}

int libuart_close(int *fd)
{
    if (*fd != -1) {
        close(*fd);
    }
    *fd = -1;
    return -1;
}

int libuart_init(int fd, int baudrate, int bits, int parity, int stop)
{
    struct termios newtio, oldtio;

    /* 获取当前串口参数 */
    if (tcgetattr(fd, &oldtio) != 0) {
        printfs("%s tcgetattr termios error!\n", strerror(errno));
    }
    bzero(&newtio, sizeof(newtio));

    /* 选择原始输入模式 */
    newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    /* 禁用软件流控制 */
    newtio.c_iflag &= ~(IXON | IXOFF | IXANY);

    /* Choosing Raw Output */
    newtio.c_oflag &= ~OPOST;	

    /* Local line - do not change "owner" of port. Enable receiver. */
    newtio.c_cflag |= CLOCAL | CREAD;  

    /* 设置波特率 */
    __libuart_set_baudrate(fd, &newtio, baudrate);

    /* 设置数据位数 */
    __libuart_set_termios_bits(&newtio, bits);

    /* 设置奇偶检验位 */
    __libuart_set_termios_parity(&newtio, parity);

    /* 设置停止位 */
    __libuart_set_termios_stop(&newtio, stop);

    newtio.c_cc[VMIN]   =   0;
    newtio.c_cc[VTIME]  =   0;
    tcflush(fd, TCIFLUSH);

    if (tcsetattr(fd, TCSANOW, &newtio) != 0) {
        printfs("%s tcsetattr termios error!\n", strerror(errno));
        return -1;
    }
    return 0;
}

int libuart_read_data(int fd, void *buffer, int length, int timeout)
{
    /* 设置超时时间 */
    struct timeval tv = {
        .tv_sec  = timeout / 1000,
        .tv_usec = timeout % 1000 * 1000,
    };
  
    fd_set rfds; 
    char *buff  = (char *)buffer;
    int counter = (int)0; 
    while (counter < length) {
        FD_ZERO(&rfds);  
        FD_SET(fd, &rfds);  
        int retval = select(fd + 1, &rfds, NULL, NULL, &tv);

        if (retval == -1) {  
            printfs("%s select() error!\n", strerror(errno));
            break;  
        } else if (retval > 0)  {
            if (read(fd, &buff[counter++], 1) < 0) {
                break;
            }
        } else {  
            break;  
        }
    }
  
    return counter;  
}

int libuart_send_data(int fd, const void *buffer, size_t length)
{
    return write(fd, buffer, length);
}  



// static int usart_lock(int fd, int type)
// {
//     if (fd < 0) return -1;

//     struct flock lock;
//     if (fcntl(fd, F_GETLK, &lock) < 0 ) {
//         printfs("get flock error!\n");
//         return -1;
//     }
//     lock.l_type = type;
//     if (fcntl(fd, F_SETLKW, &lock) < 0) {
// 		printf("Lock failed : type = %d\n",lock.l_type);
// 		return -1;
// 	}
//     return 0;
// }

// static int usart_isblock(int fd)
// {
//     if (fd < 0) return -1;
//     struct flock lock;
//     if (fcntl(fd, F_GETLK, &lock) < 0 ) {
//         printfs("get flock error!\n");
//         return -1;
//     }
//     if (lock.l_type == F_UNLCK) {
//         /* 串口设备文件未锁 */
//         return 0;
//     } else {
//         return 1;
//     }
// }

// static int usart_block(int fd, int mode)
// {
//     if (mode == 0) {
//         /* 设置串口模式为非阻塞, 覆盖前面open的属性 */
//         return fcntl(fd, F_SETFL, FNDELAY);
//     } else {
//         /* 设置串口模式为阻塞, 覆盖前面open的属性 */
//         return fcntl(fd, F_SETFL, 0);
//     }
// }

static void __libuart_set_baudrate(int fd, struct termios *operate, int baudrate)
{
    if (operate == NULL) return;

    int __baudrate = USART_DEFAULT_BAUDRATE;
    switch (baudrate) {  
    case 0      :  __baudrate = B0      ;break;
    case 50	    :  __baudrate = B50	    ;break;
    case 75	    :  __baudrate = B75	    ;break;
    case 110	:  __baudrate = B110    ;break;
    case 134	:  __baudrate = B134    ;break;
    case 150	:  __baudrate = B150    ;break;
    case 200	:  __baudrate = B200    ;break;
    case 300	:  __baudrate = B300    ;break;
    case 600	:  __baudrate = B600    ;break;
    case 1200	:  __baudrate = B1200   ;break;
    case 1800	:  __baudrate = B1800   ;break;
    case 2400	:  __baudrate = B2400   ;break;
    case 4800	:  __baudrate = B4800   ;break;
    case 9600	:  __baudrate = B9600   ;break;
    case 19200  :  __baudrate = B19200  ;break;
    case 38400  :  __baudrate = B38400  ;break;
    case 57600  :  __baudrate = B57600  ;break;
    case 115200 :  __baudrate = B115200 ;break;
    case 230400 :  __baudrate = B230400 ;break;
    case 460800 :  __baudrate = B460800 ;break;
    case 500000 :  __baudrate = B500000 ;break;
    case 576000 :  __baudrate = B576000 ;break;
    case 921600 :  __baudrate = B921600 ;break;
    case 1000000:  __baudrate = B1000000;break;
    case 1152000:  __baudrate = B1152000;break;
    case 1500000:  __baudrate = B1500000;break;
    case 2000000:  __baudrate = B2000000;break;
    case 2500000:  __baudrate = B2500000;break;
    case 3000000:  __baudrate = B3000000;break;
    case 3500000:  __baudrate = B3500000;break;
    case 4000000:  __baudrate = B4000000;break;
    default : 
    printfs("wring : baudrate not support. Use default parameters(%s).\n", 
            USART_DEFAULT_BAUDRATE_NAME);break; 
    }
    cfsetispeed(operate, __baudrate);
    cfsetospeed(operate, __baudrate);
}

static void __libuart_set_termios_bits(struct termios *operate, int bits)
{
    if (operate == NULL) return;

    operate->c_cflag &= ~CSIZE;
    switch (bits) {
    case 5:  operate->c_cflag |= CS5; break;  
    case 6:  operate->c_cflag |= CS6; break;  
    case 7:  operate->c_cflag |= CS7; break;  
    case 8:  operate->c_cflag |= CS8; break;  
    default : 
        printfs("wring : bits not support. Use default parameters(%s).\n", 
                USART_DEFAULT_BITS_NAME);
        operate->c_cflag |= USART_DEFAULT_BITS; break;
    }  
}

static void __libuart_set_termios_parity(struct termios *operate, int parity)
{
    switch (parity) {  
    case 'O':                     //奇校验  
        operate->c_cflag |= PARENB;  
        operate->c_cflag |= PARODD;  
        operate->c_iflag |= (INPCK | ISTRIP);  
        break;  
    case 'E':                     //偶校验  
        operate->c_iflag |= (INPCK | ISTRIP);  
        operate->c_cflag |= PARENB;  
        operate->c_cflag &= ~PARODD;  
        break;  
    case 'N':                    //无校验  
        operate->c_cflag &= ~PARENB;  
        break;
    default : 
        printfs("wring : parity not support. Use default parameters(%s).\n", 
                USART_DEFAULT_PARITY_NAME);
        operate->c_cflag &= ~PARENB; break;
    }
}

static void __libuart_set_termios_stop(struct termios *operate, int stop)
{
    switch(stop) {
    case USART_STOP_1 : operate->c_cflag &= ~CSTOPB; break;
    case USART_STOP_2 : operate->c_cflag |= CSTOPB;  break;
    default : 
        printfs("wring : stop bits not support. Use default parameters(%s).\n", 
                USART_DEFAULT_STOP_NAME);
        operate->c_cflag &= ~CSTOPB; break;
    }
}
