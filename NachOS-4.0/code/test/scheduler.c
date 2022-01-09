#include "syscall.h"


void main()
{
	int numStudent, outputFile, contactFile, i, j;
	char* res;
	char id;
	int IDs[10];

	CreateSemaphore("WaterTap", 1);
	CreateSemaphore("send", 0);
	CreateSemaphore("recv", 0);
	CreateSemaphore("begin", 0);
	CreateFile("output.txt");
	CreateFile("contact.txt");

	PrintString("Nhap so sinh vien: ");
	numStudent = ReadNum();

	for (i=1; i<=numStudent; i++) {
		IDs[i] = Exec("student");

		id = (char)(i + '0');
		PrintString(id);
		contactFile = Open("contact.txt", 0);
		Write(&id, 1, contactFile);
		Close(contactFile);

		Signal("send");
		Wait("recv");
	}

	PrintString("-------------------------------\n");
	for (i=1; i<=numStudent; i++) Signal("begin");

	for (i=1; i<=numStudent; i++) 
		if (IDs[i] >= 0)
			Join(IDs[i]);

	PrintString("-------------------------------\n");

	outputFile = Open("output.txt", 0);
	PrintString("\nThu tu lay nuoc la: \n");
	while(Read(res, 1, outputFile) > 0) PrintString(res);
	PrintChar('\n');

	Close(outputFile);

    Exit(0);
}
