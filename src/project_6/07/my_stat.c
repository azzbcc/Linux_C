/*
 * 文件信息获取
 */

#include <common.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    struct stat buf;

    // 检查参数个数
    if (argc != 2)
    {
        my_err("Usage：my_stat <filename>", __LINE__);
    }

    // 获取文件属性
    if (stat(argv[1], &buf) == -1)
    {
        my_err("stat", __LINE__);
    }

    // 打印文件属性
    printf("device is：%d\n", buf.st_dev);
    printf("mode is：%o\n", buf.st_mode);
    printf("number of hard links is：%d\n", buf.st_nlink);
    printf("user ID of owner is：%d\n", buf.st_uid);
    printf("group ID of owner is：%d\n", buf.st_gid);
    printf("device type (if inode device) is：%d\n", buf.st_rdev);

    printf("total size, in bytes is：%d\n", buf.st_size);
    printf("block size for filesystem I/O is：%d\n", buf.st_blksize);
    printf("number of blocks allocated is：%d\n", buf.st_blocks);

    printf("time of last access is：%s", ctime(&buf.st_atim));
    printf("time of last modification is：%s", ctime(&buf.st_mtim));
    printf("time of last change is：%s", ctime(&buf.st_ctim));

    return 0;
}