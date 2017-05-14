/*
 * 改变文件访问权限
 */

#include <common.h>
#include <sys/stat.h>

static bool judge(int n) { return 0 <= n && n < 8; }

int main(int argc, char *argv[])
{
    int mode;     // 用户输入权限
    int mode_u;   // 当前用户权限
    int mode_g;   // 当前用户组权限
    int mode_o;   // 其他用户权限

    if (argc < 3)
    {
        perror("At least 02 arguments needed\n");
        exit(EXIT_FAILURE);
    }

    int tmp_mode = atoi(argv[1]);

    mode_u = tmp_mode / 100;
    mode_g = tmp_mode / 10 % 10;
    mode_o = tmp_mode % 10;
    if (!judge(mode_u) || !judge(mode_g) || !judge(mode_o))
    {
        perror("Invalid mode\n");
        exit(EXIT_FAILURE);
    }

    mode = (mode_u << 6) + (mode_g << 3) + mode_o;

    if (chmod(argv[2], (__mode_t) mode))
    {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(errno);
    }

    return 0;
}