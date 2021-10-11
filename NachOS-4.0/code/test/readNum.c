#include "syscall.h"

int
main()
{
    int result;
    result = ReadNum();
    PrintNum(result);
    Halt();
    /* not reached */
}