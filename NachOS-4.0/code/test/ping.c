#include "syscall.h"

int main()
{
	int i,j,r;
	for(i =0; i< 1000; i++)
	{
		Wait("write");
		PrintChar('A');
		Signal("write");
		//r = RandomNum() % 10000;
		//for(j =0; j< r; j++);
	}
	Exit(0);	
}
