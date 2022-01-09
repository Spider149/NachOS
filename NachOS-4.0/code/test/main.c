#include "syscall.h"

int main(){
    int fileID;
    char c;
    char* a;
    char* tmp;
    c =  'a';


    PrintNum(Read(a, 1, 0));

    PrintChar('\n');
    PrintString(a);
    PrintChar('\n');

    Halt();
}

