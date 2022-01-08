#include "syscall.h"


void main()
{
	int pingPID, pongPID;
	PrintString("Ping-Pong test starting...\n\n");
	pingPID = Exec("ping");
	pongPID = Exec("pong");
	PrintString("chay duoc exec\n");
	Join(pingPID);
	Join(pongPID);	
}
