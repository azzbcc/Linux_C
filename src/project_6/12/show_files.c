/*
 * 目录信息
 */

#include <common.h>
#include <dirent.h>

int my_readir(const char *path)
{
    DIR             *dir;
    struct dirent   *ptr;

    if ((dir = opendir(path)) == NULL)
    {
        perror("opendir");
        return -1;
    }

    while ((ptr = readdir(dir)) != NULL)
    {
        printf("file name：%s\n", ptr->d_name);
    }

    closedir(dir);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        my_err("listfile <target ls_path>", __LINE__);
    }

    if (my_readir(argv[1]) < 0)
    {
        my_err("readdir failed", __LINE__);
    }

    return 0;
}