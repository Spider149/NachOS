#include "syscall.h"

int main(){
    int fileID;
    char c;
    char* a;
    char* tmp;
    c =  'a';
    
    fileID = Open("output.txt", 0);
    PrintNum(Read(tmp, 300, fileID));
    PrintString(tmp);
    tmp = "a23";
    PrintNum(Write(tmp, 3, fileID));
    Close(fileID);


    PrintNum(Read(a, 1, 0));

    PrintChar('\n');
    PrintString(a);
    PrintChar('\n');

    Halt();
}

