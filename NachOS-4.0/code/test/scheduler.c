#include "syscall.h"


void main()
{
	int numStudent, i, j;
	//char* tmp;
	int IDs[10];

	CreateSemaphore("WaterTap", 1);
	CreateFile("output.txt");

	//Read(tmp, 1, 0);
	//numStudent = (int) (tmp[1] - '0');
	numStudent = ReadNum();

	for (i=0; i<numStudent; i++){
		IDs[i] = Exec("student");
	}
	
	for (i=0; i<numStudent; i++){
		Join(IDs[i]);
	}
}
