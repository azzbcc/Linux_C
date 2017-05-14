/*
 * 设置目录
 */

#include <common.h>
#include <zconf.h>

int main(int argc, char *argv[])
{
    char buf[PATH_MAX + 1];

    if (argc < 2)
    {
        my_err("my_cd <target ls_path>\n", __LINE__);
    }

    if (chdir(argv[1]) < 0)
    {
        my_err("chdir", __LINE__);
    }

    if (getcwd(buf, PATH_MAX + 1) < 0)
    {
        my_err("getcwd", __LINE__);
    }

    printf("%s\n", buf);

    return 0;
}