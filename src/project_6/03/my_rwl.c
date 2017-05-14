/*
 * 文件读写
 */

#include <fcntl.h>
#include <common.h>
#include <unistd.h>

// 自定义的读数据函数
int my_read(int fd)
{
    off_t len;
    off_t ret;
    char buf[64];

    // 获取文件长度并保持文件读写指针在文件开始处
    if (-1 == lseek(fd, 0, SEEK_END))
    {
        my_err("lseek", __LINE__);
    }
    if (-1 == (len = lseek(fd, 0, SEEK_CUR)))
    {
        my_err("lseek", __LINE__);
    }
    if (-1 == lseek(fd, 0, SEEK_SET))
    {
        my_err("lseek", __LINE__);
    }

    printf("len：%li\n", len);
    // 读数据
    if ((ret = read(fd, buf, len)) < 0)
    {
        my_err("read", __LINE__);
    }

    // 打印数据
    for (int i = 0; i < len; ++i)
    {
        printf("%c", buf[i]);
    }
    printf("\n");

    return ret;
}

int main(int argc, char *argv[])
{
    char buf[] = "Hello World";

//    int fd = creat("test", S_IRWXU);
    int fd = open("test", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);

    if (fd == -1)
    {
        my_err("open", __LINE__);
    }
    else
    {
        printf("create file success\n");
    }

    // 写数据
    if (write(fd, buf, strlen(buf)) != strlen(buf))
    {
        my_err("write", __LINE__);
    }
    my_read(fd);

    // 演示文件的间隔
    printf("=======================\n");

    if (-1 == lseek(fd, 5, SEEK_END))
    {
        my_err("lseek", __LINE__);
    }

    // 写数据
    if (write(fd, buf, strlen(buf)) != strlen(buf))
    {
        my_err("write", __LINE__);
    }
    my_read(fd);

    close(fd);

    return 0;
}