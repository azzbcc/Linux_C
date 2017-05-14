/*
 * 文件屏蔽权限
 */

#include <fcntl.h>
#include <common.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    system("rm test1 test2");

    umask(0);       // 不屏蔽任何权限
    if (open("test1", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO) < 0)
    {
        my_err("open test1", __LINE__);
    }

    umask(S_IRWXO); // 屏蔽其他用户的所有权限
    if (open("test2", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO) < 0)
    {
        my_err("open test2", __LINE__);
    }
    system("ls -alh --color=auto test1 test2");

    return 0;
}