#include "syscall.h"


void main()
{
    int i, r, j, id;

    id = GetPID();

    for (i=0; i<10; i++){
        Wait("WaterTap");

        r = RandomNum() % 100000;
        for (j=0; j<r; j++);

        PrintNum(id);
        PrintString("\n");

        if (i==9){
            PrintString("--- Sinh vien ");
            PrintNum(id);
            PrintString(" da lay nuoc xong ---\n");
        }

        Signal("WaterTap");
    }
    Exit(0);
}
