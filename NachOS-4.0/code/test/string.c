#include "syscall.h"
#define MAX_LENGTH 1000

int
main()
{
    char* str;
    PrintString("Viet Nam\n");
    ReadString(str, MAX_LENGTH);
    PrintString(str);
    Halt();
    /* not reached */
}
