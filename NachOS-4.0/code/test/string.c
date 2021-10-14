#include "syscall.h"
#define MAX_LENGTH 1000

int
main()
{
    char* str;
    PrintString("Nhap 1 chuoi ky tu:\n");
    ReadString(str, MAX_LENGTH);
    PrintString("Chuoi ky tu vua nhap la:\n");
    PrintString(str);
    Halt();
    /* not reached */
}
