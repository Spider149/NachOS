#include "syscall.h"

int
main()
{
    char c;
    PrintString("Nhap 1 ky tu:\n");
    c = ReadChar();
    PrintString("Ky tu vua nhap la:\n");
    PrintChar(c);
    Halt();
    /* not reached */
}
