#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int i;
    int j = 0;
    char *pi_buf[32];
    char total_buf[156];
    char *buf = total_buf;
    char *pi;
    pi = buf;

    for(i = 1; i < argc; i ++)
    {
        pi_buf[j++] = argv[i];
    }

    int len;
    int sum = 0;
    while((len = read(0, buf, 32)) > 0)
    {
        sum += len;
        if(sum > 256)
        {
            fprintf(2, "args too long!\n");
            exit(0);
        }
        for(i = 0; i < len; i ++)
        {
            if(buf[i] == ' ')
            {
                buf[i] = 0;
                pi_buf[j ++] = pi;
                pi = &buf[i + 1];
            }
            else if(buf[i] == '\n')
            {
                buf[i] = 0;
                pi_buf[j ++] = pi;
                pi = &buf[i + 1];
                pi_buf[j] = 0;
                if(fork() == 0)
                {
                    exec(argv[1], pi_buf);
                    exit(0);
                }
                else
                {
                    j = argc - 1;
                    wait(0);
                }
            }
        }
        buf = buf + len;
    }
    exit(0);
    return 0;
}