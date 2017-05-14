//
// Created by azzbcc on 17-5-11.
//

#include <common.h>

int min(int a, int b) { return a < b ? a : b; }

int max(int a, int b) { return a > b ? a : b; }

void itoa(int n, char *str, int base)
{
    assert(str != NULL);

    char *p = str, tmp[65];
    if (n < 0)
    {
        n = -n;
        *p++ = '-';
    }
    *p = '\0';

    p = tmp;
    while (n)
    {
        *p++ = (char) (n % base + '0');
        n /= base;
    }
    *p = '\0';

    strcat(str, reserve(tmp));
}

char *reserve(char *str)
{
    assert(str != NULL);

    char *beg = str, *end = str;
    while ('\0' != *end) end++;
    end--;
    while (beg < end)
    {
        char tmp;
        tmp = *beg, *beg = *end, *end = tmp;
        beg++, end--;
    }
    return str;
}

void my_err(const char *err_string, int line)
{
    fprintf(stderr, "line：%d ", line);
    perror(err_string);
    exit(EXIT_FAILURE);
}