/*
 * 文件移动
 */

#include <common.h>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        my_err("my_mv <old_file> <new_file>", __LINE__);
    }

    if (rename(argv[1], argv[2]) < 0)
    {
        my_err("rename", __LINE__);
    }

    return 0;
}