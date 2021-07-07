#include "libsystem.h"

struct pid_tl {
    pid_t pid;
    struct pid_tl *next;
};

struct pid_tl *haed_pid = NULL;
static int pid_add_new(struct pid_tl **pidl, int pid) ;
static int pid_delete(struct pid_tl **pidl, int pid) ;

int systemos(const char *cmdstring)
{
    pid_t pid;
    int status;

    if (cmdstring == NULL) {
        return 1;
    }

    if ((pid = fork()) < 0) {
        status = -1;
    } else if (pid == 0) {
        /* child process  */
        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
        exit(127);  /* 子进程正常情况下不会执行此语句 */
    } else {
        /* parent process */
        pid_add_new(&haed_pid, pid);
        while(waitpid(pid, &status, 0) < 0) {
            if (errno != EINTR) {
                status = -1; break;
            }
        }
        pid_delete(&haed_pid, pid);
    }
    return status;
}

int systemosp(const char *cmdstring, char *buf, size_t len)
{
    int   fd[2];
    pid_t pid;
    int   n, count; 
    int status;

    memset(buf, 0, len);
    if (pipe(fd) < 0) {
        return -1;
    }

    if ((pid = fork()) < 0) {
        status = -1;
    } else if (pid == 0) {
        /* child process  */
        close(fd[0]);     /* close read end */
        if (fd[1] != STDOUT_FILENO) {
            if (dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO) {
                return -1;
            }
            close(fd[1]);
        }
        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
        exit(127);  /* 子进程正常情况下不会执行此语句 */
    } else {   
        /* parent process */
        close(fd[1]);     /* close write end */
        count = 0;

        char buff[100];
        do {
            memset(buff, 0, 100);
            n = read(fd[0], buff, 100);
            // printf("%s",buff);
            if ((count + n) > len) {
                memcpy(buf + count, buff, len - count);
                break;
            }
            memcpy(buf + count, buff, n);
            count += n;
        }while(waitpid(pid, &status, WNOHANG) == 0);

        close(fd[0]);

        if (waitpid(pid, &status, WNOHANG) == 0) {
            return -1;
        }
    }
    return 0;
}


static int pid_add_new(struct pid_tl **pidl, int pid) 
{
    struct pid_tl *new = malloc(sizeof(struct pid_tl));
    if (new == NULL) {
        return -1;
    }

    new->pid = pid;
    new->next = *pidl;
    *pidl = new;
    return 0;
}

static int pid_delete(struct pid_tl **pidl, int pid) 
{
    for (struct pid_tl **curr = pidl; *curr; ) {
        struct pid_tl *entry = *curr;
        if (entry->pid == pid) {
            *curr = entry->next;
            free(entry);
        } else {
            curr = &entry->next;
        }
    }
    return 0;
}
