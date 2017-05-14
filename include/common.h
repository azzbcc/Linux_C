#ifndef LINUXC_COMMON_H
#define LINUXC_COMMON_H

#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int min(int, int);

int max(int, int);

void itoa(int, char *, int);
// 字符串逆置
char *reserve(char *);
// 打印错误日志
void my_err(const char *, int);

#endif //LINUXC_COMMON_H
