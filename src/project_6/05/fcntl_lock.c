/*
 * 文件锁的初步认识
 */

#include <fcntl.h>
#include <common.h>
#include <unistd.h>

// 锁的设置或释放函数
static int lock_set(int fd, struct flock *lock)
{
    if (0 == fcntl(fd, F_SETLK, lock))
    {
        if (lock->l_type == F_RDLCK)
        {
            printf("set read lock, pid：%d\n", getpid());
        }
        if (lock->l_type == F_WRLCK)
        {
            printf("set write lock, pid：%d\n", getpid());
        }
        if (lock->l_type == F_UNLCK)
        {
            printf("release lock, pid：%d\n", getpid());
        }
    }
    else
    {
        perror("lock operation failed\n");
        return -1;      // 执行失败，返回-01
    }

    return 0;
}

// 测试锁，只有当测试发现参数lock指定的锁能被设置时，返回0
static int lock_test(int fd, struct flock *lock)
{
    if (0 == fcntl(fd, F_GETLK, lock))
    {
        if (lock->l_type == F_UNLCK)
        {
            printf("lock can be set in fd\n");
            return 0;
        }
        else
        {       // 有不兼容的锁存在，打印处设置该锁的进程
            if (lock->l_type == F_RDLCK)
            {
                printf("can't set lock, read lock has been set by：%d\n", lock->l_pid);
            }
            else if (lock->l_type == F_WRLCK)
            {
                printf("can't set lock, write lock has been set by：%d\n", lock->l_pid);
            }

            return -2;
        }
    }
    else
    {
        perror("get incompatible locks failed\n");
        return -1;
    }
}

int main(int argc, char *argv[])
{
    struct flock lock;
    char         buf[32];

    int fd = open("test", O_CREAT | O_TRUNC | O_RDWR, S_IRWXU);
    if (fd == -1)
    {
        my_err("open", __LINE__);
    }

    if (write(fd, "test lock", 10) != 10)
    {
        my_err("write", __LINE__);
    }

    // 初始化 lock 结构
    memset(&lock, 0, sizeof(struct flock));
    lock.l_len    = 0;
    lock.l_start  = SEEK_SET;
    lock.l_whence = 0;

    // 设置读锁
    lock.l_type = F_RDLCK;
    if (0 == lock_test(fd, &lock))
    {   // 测试可以设置锁
        lock.l_type = F_RDLCK;
        lock_set(fd, &lock);
    }

    // 读数据
    lseek(fd, 0, SEEK_SET);
    int ret = read(fd, buf, 10);
    if (ret < 0)
    {
        my_err("read", __LINE__);
    }
    buf[ret] = '\0';
    printf("%s\n", buf);

    getchar();

    // 设置写锁
    lock.l_type = F_WRLCK;
    if (0 == lock_test(fd, &lock))
    {
        lock.l_type = F_WRLCK;
        lock_set(fd, &lock);
    }

    lock.l_type = F_UNLCK;
    lock_set(fd, &lock);

    close(fd);

    return 0;
}