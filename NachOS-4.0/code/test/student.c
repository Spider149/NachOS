#include "syscall.h"


void main()
{
    int id, waitTime, fileID, size_out, i, j, k;
    char* out;
    char* tmp;

    id = GetPID();
    out = "  ";
    size_out = 2;
    out[0] = (char)(id + '0');

    fileID = Open("output.txt", 0);

    for (i=0; i<10; i++){
        Wait("WaterTap");

        waitTime = RandomNum() % 100000;
        for (j=0; j<waitTime; j++);

        for (k=1; k<id; k++) PrintString("      ");
        PrintNum(id);
        PrintString("\n");

        while(Read(tmp, 1, fileID) > 0);
        Write(out, size_out, fileID);

        Signal("WaterTap");
    }
    Close(fileID);
    
    PrintString("(Student ");
    PrintNum(id);
    PrintString(" done)\n");

    Exit(0);
}
