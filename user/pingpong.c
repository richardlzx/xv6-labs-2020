#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
    int fd_p2c[2];
    int fd_c2p[2];
    char buf = 'P';
    pipe(fd_c2p);
    pipe(fd_p2c);
    int pid = fork();
    int exit_status = 0;
    if(pid < 0)
    {
        fprintf(2, "fork error!\n");
        close(fd_c2p[0]);
        close(fd_c2p[1]);
        close(fd_p2c[0]);
        close(fd_p2c[1]);
        exit(1);
    }
    if(pid == 0)
    {
        close(fd_p2c[1]);
        close(fd_c2p[0]);
        if(read(fd_p2c[0], &buf, sizeof(buf)) != sizeof(buf))
        {
            fprintf(2, "child read error!\n");
            exit_status = 1;
        }
        else
        {
           fprintf(1, "%d: received ping\n", getpid()); 
        }
        if(write(fd_c2p[1], &buf, sizeof(buf)) != sizeof(buf))
        {
            fprintf(2, "child write error!\n");
            exit_status = 1;
        }
        close(fd_c2p[1]);
        close(fd_p2c[0]);
        exit(exit_status);
    }
    else
    {
        close(fd_p2c[0]);
        close(fd_c2p[1]);
        if(write(fd_p2c[1], &buf, sizeof(buf)) != sizeof(buf))
        {
            fprintf(2, "parent write error!\n");
            exit_status = 1;
        }
        if(read(fd_c2p[0], &buf, sizeof(buf)) != sizeof(buf))
        {
            fprintf(2, "parent read error!\n");
            exit_status = 1;
        }
        else
        {
            fprintf(1, "%d: received pong\n", getpid());
        }
        close(fd_p2c[1]);
        close(fd_c2p[0]);
        exit(exit_status);
    }
}