#include "syscall.h"


void main()
{
    int waitTime, outputFile, contactFile, i, j, k;
    char id;
    char* tmp;

    Wait("send");
    contactFile = Open("contact.txt", 0);
    Read(&id, 1, contactFile);
	Close(contactFile);
    Signal("recv");
    //PrintString("Here");
    Wait("begin");

    outputFile = Open("output.txt", 0);

    for (i=0; i<10; i++){
        Wait("WaterTap");

        waitTime = RandomNum() % 100000;
        for (j=0; j<waitTime; j++);

        for (k=1; k<(int)(id-'0'); k++) PrintString("      ");
        PrintChar(id);
        PrintString("\n");

        while(Read(tmp, 1, outputFile) > 0);
        tmp = " ";
        Write(&id, 1, outputFile);
        Write(tmp, 1, outputFile);

        Signal("WaterTap");
    }
    Close(outputFile);
    
    PrintString("(Student ");
    PrintChar(id);
    PrintString(" done)\n");

    Exit(0);
}
