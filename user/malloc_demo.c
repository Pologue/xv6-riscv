#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
    if (argc > 0)
    {
        return malloc_demo((uint64)argv[1]) ;
    }
    
    return -1;
}
