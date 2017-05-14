/*
 * 创建文件函数
 */

#include <fcntl.h>
#include <common.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
//    int fd = open("test", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    int fd = creat("test", S_IRWXU);

    if (fd == -1)
    {
        fprintf(stderr, strerror(errno));
        exit(EXIT_FAILURE);
    }

    close(fd);

    return 0;
}