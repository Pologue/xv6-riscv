#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

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
        addr = atoi(argv[i]);
        if (addr < 0)
        {
            fprintf(2, "free_demo: cannot free %s bytes\n", addr);
            exit(1);
        }
        free_demo(addr);
    }
    exit(0);
}
