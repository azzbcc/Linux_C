//
// 自己实现ls
//

#include <unistd.h>
#include <common.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

#define MAX 128

typedef struct __tag_file_info
{
    __ino_t         d_ino;                      // inode number 索引节点号
    char            authority[11];              // 文件权限字符串
    __nlink_t       st_nlink;                   // 文件的硬连接数
    char            user_name[NAME_MAX + 1];    // 用户字符串
    char            group_name[NAME_MAX + 1];   // 用户组字符串Q5
    __off_t         st_size;                    // 以字节为单位的文件容量
    struct timespec st_mtim;                    // 文件最后一次修改时间
    char            d_name[NAME_MAX + 1];       // 文件名
    char            real_name[NAME_MAX + 1];    // link文件所指向文件名
} FileInfo;

FileInfo    fileInfoList[MAX];
static char *ls_path = "."; // ls执行路径
static bool ARG_A    = false;
static bool ARG_I    = false;
static bool ARG_L    = false;
static bool ARG_R    = false;
static bool ARG_T    = false;

// 真实执行结果
void real()
{
    char command[60] = "ls ";
    if (ARG_A) strcat(command, "-a ");
    if (ARG_I) strcat(command, "-i ");
    if (ARG_L) strcat(command, "-l ");
    if (ARG_R) strcat(command, "-r ");
    if (ARG_T) strcat(command, "-t ");
    strcat(command, ls_path);

    printf("%s\n", command);
    system(command);
}

// 初始化命令行参数
void init_opt(int argc, char *argv[])
{
    int nc;   // 命令行参数

    while ((nc = getopt(argc, argv, "ailrt")) != -1)
    {
        switch (nc)
        {
        case 'a':
            ARG_A = true;
            break;
        case 'i':
            ARG_I = true;
            break;
        case 'l':
            ARG_L = true;
            break;
        case 'r':
            ARG_R = true;
            break;
        case 't':
            ARG_T = true;
            break;
        case '?':
            printf("无效的选项字符'%c'\n", optopt);
            break;
        default:
            break;
        }
    }
    if (NULL != argv[optind]) ls_path = argv[optind];
}

void read_file_info(char file_name[], FileInfo *pInfo)
{
    struct stat buf;

    // 获取文件属性
    if (lstat(file_name, &buf) == -1)
    {
        my_err("stat", __LINE__);
    }

    // 文件类型
    if (S_ISLNK(buf.st_mode))
    {
        strcpy(pInfo->authority, "l");
        if (readlink(file_name, pInfo->real_name, sizeof(pInfo->real_name)) < 0)
        {
            my_err("readlink", __LINE__);
        }
    }
    if (S_ISREG(buf.st_mode)) strcpy(pInfo->authority, "-");
    if (S_ISDIR(buf.st_mode)) strcpy(pInfo->authority, "d");
    if (S_ISCHR(buf.st_mode)) strcpy(pInfo->authority, "c");
    if (S_ISBLK(buf.st_mode)) strcpy(pInfo->authority, "b");
    if (S_ISFIFO(buf.st_mode)) strcpy(pInfo->authority, "f");
    if (S_ISSOCK(buf.st_mode)) strcpy(pInfo->authority, "s");

    // 文件权限
    strcat(pInfo->authority, buf.st_mode & S_IRUSR ? "r" : "-");
    strcat(pInfo->authority, buf.st_mode & S_IWUSR ? "w" : "-");
    strcat(pInfo->authority, buf.st_mode & S_IXUSR ? "x" : "-");
    strcat(pInfo->authority, buf.st_mode & S_IRGRP ? "r" : "-");
    strcat(pInfo->authority, buf.st_mode & S_IWGRP ? "w" : "-");
    strcat(pInfo->authority, buf.st_mode & S_IXGRP ? "x" : "-");
    strcat(pInfo->authority, buf.st_mode & S_IROTH ? "r" : "-");
    strcat(pInfo->authority, buf.st_mode & S_IWOTH ? "w" : "-");
    strcat(pInfo->authority, buf.st_mode & S_IXOTH ? "x" : "-");

    pInfo->st_nlink = buf.st_nlink;
    strcpy(pInfo->user_name, getpwuid(buf.st_uid)->pw_name);
    strcpy(pInfo->group_name, getgrgid(buf.st_gid)->gr_name);
    pInfo->st_size = buf.st_size;
    pInfo->st_mtim = buf.st_mtim;
}

int read_path()
{
    DIR           *dir;
    struct dirent *ptr;

    if ((dir = opendir(ls_path)) == NULL)
    {
        perror("opendir");
        return -1;
    }

    int ret = 0;
    while ((ptr = readdir(dir)) != NULL)
    {
        fileInfoList[ret].d_ino = ptr->d_ino;
        strcpy(fileInfoList[ret].d_name, ptr->d_name);

        char tmp_file_name[NAME_MAX + 1];
        strcpy(tmp_file_name, ls_path);
        strcat(tmp_file_name, "/");
        strcat(tmp_file_name, ptr->d_name);
        read_file_info(tmp_file_name, &fileInfoList[ret++]);
    }

    closedir(dir);

    return ret;
}

static int cmp(const void *a, const void *b)
{
    FileInfo *pa     = (FileInfo *) a, *pb = (FileInfo *) b;
    int      by_name = strcasecmp(pa->d_name, pb->d_name);
    int      by_time = (int) (pa->st_mtim.tv_sec - pb->st_mtim.tv_sec);
    return ARG_R ? (ARG_T ? by_time : -by_name) : (ARG_T ? -by_time : by_name);
}

void output(const int count)
{
    qsort(fileInfoList, count, sizeof(FileInfo), cmp);

    for (int i = 0; i < count; ++i)
    {
        if (ARG_A || '.' != fileInfoList[i].d_name[0])
        {
            char      tmp_time[MAX];
            struct tm *ptm = gmtime(&fileInfoList[i].st_mtim.tv_sec);

            strftime(tmp_time, MAX, "%F %T", ptm);

            if (ARG_I) printf("%8lu ", fileInfoList[i].d_ino);
            printf("%s %2lu %6s %6s %6li %s  %s", fileInfoList[i].authority, fileInfoList[i].st_nlink,
                   fileInfoList->user_name, fileInfoList->group_name, fileInfoList[i].st_size, tmp_time,
                   fileInfoList[i].d_name);
            if ('l' == fileInfoList[i].authority[0]) printf(" -> %s", fileInfoList[i].real_name);
            printf("\n");
        }
    }
}

int main(int argc, char *argv[])
{
    // 初始化命令行参数
    init_opt(argc, argv);

//    // 真实执行结果
//    real();

    output(read_path());

    return 0;
}