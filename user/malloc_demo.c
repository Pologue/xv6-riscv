#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
    int i;
    uint64 nbytes;

    if (argc <= 1)
    {
        malloc_demo(0);
        exit(0);
    }

    for (i = 1; i < argc; i++)
    {
        nbytes = atoi(argv[i]);
        if (nbytes < 0)
        {
            fprintf(2, "malloc_demo: cannot malloc %s bytes\n", nbytes);
            exit(1);
        }
        malloc_demo(nbytes);
    }
    exit(0);
}
