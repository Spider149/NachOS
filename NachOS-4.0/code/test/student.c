#include "syscall.h"


void main()
{
    int id, waitTime, fileID, size_out, i, j;
    char* out;
    char* tmp;

    id = GetPID();
    out = "  ";
    size_out = 2;
    out[0] = (char)(id + '0');

    for (i=0; i<10; i++){
        Wait("WaterTap");

        waitTime = RandomNum() % 100000;
        for (j=0; j<waitTime; j++);

        PrintNum(id);
        PrintString("\n");

        fileID = Open("output.txt", 0);
        Read(tmp, 300, fileID);
        Write(out, size_out, fileID);
        Close(fileID);

        Signal("WaterTap");
    }
    Exit(0);
}
