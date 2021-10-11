#include "syscall.h"

int
main()
{
    int random = RandomNum();
    PrintNum(random);
    PrintChar('\n');
    PrintNum(-1492001111);
    PrintChar('\n');
    PrintNum(123213);
    Halt();
    /* not reached */
}