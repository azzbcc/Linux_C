/*
 * 文件删除
 */

#include <common.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char buf[32];
    int fd = open("test", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
    if (fd < 0)
    {
        my_err("open test", __LINE__);
    }
    if (unlink("test") < 0)
    {
        my_err("unlink", __LINE__);
    }
    printf("file unlinked\n");

    if (write(fd, "temp", 5) < 0)
    {
        my_err("write", __LINE__);
    }
    if (lseek(fd, 0, SEEK_SET) == -1)
    {
        my_err("lseek", __LINE__);
    }
    if (read(fd, buf, 5) < 0)
    {
        my_err("read", __LINE__);
    }
    printf("%s\n", buf);

    close(fd);

    return 0;
}