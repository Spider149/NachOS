#include "syscall.h"

int
main()
{
    int n = RandomNum();
    PrintString("So nguyen ngau nhien la:\n");
    PrintNum(n);
    PrintChar('\n');
    PrintString("Nhap 1 so nguyen:\n");
    n = ReadNum();
    PrintString("So nguyen vua nhap la:\n");
    PrintNum(n);    
    PrintChar('\n');
    Halt();
    /* not reached */
}