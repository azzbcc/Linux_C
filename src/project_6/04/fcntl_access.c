/*
 * 改变文件访问权限
 */

#include <fcntl.h>
#include <common.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd = open("test", O_CREAT | O_TRUNC | O_RDWR, S_IRWXU);

    if (fd == -1)
    {
        my_err("open", __LINE__);
    }

    // 设置文件打开方式
    int ret = fcntl(fd, F_SETFL, O_APPEND);
    if (ret < 0)
    {
        my_err("fcntl", __LINE__);
    }

    // 获取文件打开方式
    ret = fcntl(fd, F_GETFL, 0);
    if (ret < 0)
    {
        my_err("fcntl", __LINE__);
    }

    char str[64];
    itoa(ret, str, 8);
    printf("%s\n", str);

    int access_mode = ret & O_ACCMODE;
    if (access_mode == O_RDONLY)
    {
        printf("test access mode：read only");
    }
    else if (access_mode == O_WRONLY)
    {
        printf("test access mode：write only");
    }
    else if (access_mode == O_RDWR)
    {
        printf("test access mode：read + write");
    }

    if (ret & O_APPEND)
    {
        printf(", append");
    }
    if (ret & O_NONBLOCK)
    {
        printf(", nonblock");
    }
    if (ret & O_SYNC)
    {
        printf(", sync");
    }
    printf("\n");

    close(fd);

    return 0;
}