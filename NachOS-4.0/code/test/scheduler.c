#include "syscall.h"


void main()
{
	int n, i, j;
	int IDs[10];

	n = 5;
	CreateSemaphore("WaterTap", 1);
	for (i=0; i<n; i++){
		IDs[i] = Exec("student");
	}
	for (i=0; i<n; i++){
		Join(IDs[i]);
	}
}
