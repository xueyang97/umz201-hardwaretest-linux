#include "play.h"

static int __speaker_play_on(void *wav);

static int __speaker_running = 0;
pthread_t play_p;

int test_speaker(int argc, char *argv[])
{
    if (argc != 3) {
        printfs("parameters error. example : %s /sdcard/play.wav on/off\n", 
                __STRING_NAME(PLAY_CMD_NAME));
        return 1;
    }

    if (strcmp(argv[2], "on") == 0 && __speaker_running == 0) {
        if (pthread_create(&play_p, NULL, (void*)__speaker_play_on, argv[1]) != 0) {
            printfs("pthread_create error.");
            return 1;
        }
        return 0;
    } else if (strcmp(argv[2], "off") == 0) {
        pthread_cancel(play_p);
        return 0;
        // if (__speaker_running == 0) {
        //     return 0;
        // } else {
        //     __speaker_running = 0;
        //     return pthread_cancel(play_p);
        // }
    }
    
    return 1;
}

static int __speaker_play_on(void *wav)
{
    __speaker_running = 1;
    char buffer[100];
    vsprintfs(buffer, "aplay %s", (char *)wav);
    // int retval;
    // while((retval = systemos((const char *)buffer)) == 0) {
    //     sleep(1);
    // }
    int retval = systemos((const char *)buffer);
    __speaker_running = 0;
    return retval;
}
