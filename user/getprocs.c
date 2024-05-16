// Invoke getprocs() and print the result

#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char const *argv[])
{
    int count = getprocs();
    printf("There are %d active processes.\n", count);
    exit(0);
}
