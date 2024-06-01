#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

uint64
xtoi(const char *s)
{
    uint64 n;
    int delta;

    n = 0;
    while (('0' <= *s && *s <= '9') ||
           ('a' <= *s && *s <= 'f') ||
           ('A' <= *s && *s <= 'F'))
    {
        if (*s >= '0' && *s <= '9')
        {
            delta = *s++ - '0';
        }
        else if (*s >= 'a' && *s <= 'f')
        {
            delta = *s++ - 'a' + 10;
        }
        else
        {
            delta = *s++ - 'A' + 10;
        }
        n = n * 16 + delta;
    }
    return n;
}

int main(int argc, char const *argv[])
{
    int i;
    uint64 addr;

    if (argc <= 1)
    {
        printf("no attribute\n");
        exit(0);
    }

    for (i = 1; i < argc; i++)
    {
        addr = xtoi(argv[i]);
        free_demo(addr);
    }
    exit(0);
}
