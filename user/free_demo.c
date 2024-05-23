#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
    if (argc > 0)
    {
        return free_demo();
    }
    
    return -1;
}
