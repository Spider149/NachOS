#include "syscall.h"

int main(){
    int success = CreateFile("output.txt");
    if(success == -1) PrintString("Failed\n");
    else PrintString("Success\n");
    Halt();
}