#include "syscall.h"

int main(){
    //CreateFile("output2.txt");
    char c;
    char* a;
    int id = Open("output.txt", 0);
    c =  'a';

    PrintNum(Write(&c, 1, id));
    PrintChar('\n');
    PrintNum(Read(a, 1, id));

    PrintChar('\n');
    PrintString(a);
    PrintChar('\n');

    PrintNum(Close(id));
    Halt();
}