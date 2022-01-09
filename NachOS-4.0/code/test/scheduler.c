#include "syscall.h"


void main()
{
	int numStudent, fileID, i, j;
	char* res;
	int IDs[10];

	CreateSemaphore("WaterTap", 1);
	CreateFile("output.txt");
	fileID = Open("output.txt", 0);

	PrintString("Nhap so sinh vien: ");
	numStudent = ReadNum();

	//PrintString("-------------------------------\n");

	for (i=0; i<numStudent; i++) IDs[i] = Exec("student");
	for (i=0; i<numStudent; i++) 
		if (IDs[i] >= 0)
			Join(IDs[i]);

	//PrintString("-------------------------------\n");

	PrintString("Thu tu lay nuoc la: \n");
	while(Read(res, 1, fileID) > 0) PrintString(res);
	PrintChar('\n');

	Close(fileID);
}
