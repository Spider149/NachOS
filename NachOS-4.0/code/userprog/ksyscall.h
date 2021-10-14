/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__
#define __USERPROG_KSYSCALL_H__
#define MAX_LENGTH 1000
#define MAX_INT 2147483647

#include "kernel.h"
#include "synchconsole.h"
#include <stdlib.h>
#include <time.h>

void SysHalt()
{
  kernel->interrupt->Halt();
}

int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

int SysRandomNum()
{
  srand(time(NULL));
  int r = rand() % 2147483647 + 1;
  return r;
}

void SysPrintNum(int Number)
{
  long long number = Number;
  int lastNum, index = 0, i = 0;
  char arr[100] = ""; //MAX_INT 2147483647
  if (number < 0)
  {
    number *= -1;
    kernel->synchConsoleOut->PutChar('-');
  }

  do
  {
    lastNum = number % 10;
    arr[index++] = lastNum;
    number = number / 10;
  } while (number != 0);

  while (i < index)
  {
    kernel->synchConsoleOut->PutChar(arr[index - 1 - i] + '0');
    i++;
  }
}

int SysReadNum()
{
  long long result = 0;
  int sign = 0;

  while (true) {
    char temp = kernel->synchConsoleIn->GetChar();
    if(temp == '-' && sign == 0){
      sign = -1;
    }
    else if(temp == '+' && sign == 0){
      sign = 1;
    }
    else if('0' <= temp && temp <= '9'){
      if(sign == 0)
        sign = 1;
      result = result * 10 + (temp - '0') * sign;
      if (result > MAX_INT)
        return MAX_INT;
      if (result < -MAX_INT - 1)
        return -MAX_INT - 1;
    }
    else if(temp == '\n') {
      return result;
    }
    else return 0;
  }
}

void SysPrintChar(char character)
{
  kernel->synchConsoleOut->PutChar(character);
}

char SysReadChar()
{
  char c;
  c = kernel->synchConsoleIn->GetChar();
  return c;
}

void System2User(int virtAddr, int length, char* buffer) {
  int i = 0;
  int temp = 0 ;
  do {
    temp= (int) buffer[i];
    kernel->machine->WriteMem(virtAddr+i,1,temp);
    ++i;
  } while(i < length && temp != 0);
} 

void SysReadString() {
  int length = kernel->machine->ReadRegister(5);
	char* sysBuffer = new char[length + 1];

	int i = 0;
	while (i < length){
		char ch = kernel->synchConsoleIn->GetChar();
		if (ch == '\n' || ch == 0){
			sysBuffer[i] = '\0';
			break;
		}
		sysBuffer[i] = ch;
    i++;
	}
  sysBuffer[i] = '\0';
	System2User(kernel->machine->ReadRegister(4), i+1, sysBuffer);
	delete[] sysBuffer;
}

char* User2System(int virtAddr,int limit) {
  int temp;
  char* kernelBuffer = new char[limit +1];
  if (kernelBuffer == NULL)
    return kernelBuffer;
  
  for (int i = 0 ; i < limit;i++)
  {
    kernel->machine->ReadMem(virtAddr + i,1,&temp);
    kernelBuffer[i] = (char)temp;
    if (temp == 0)
      break;
  }
  return kernelBuffer;
}

void SysPrintString() {
  char* sysBuffer = User2System(kernel->machine->ReadRegister(4), MAX_LENGTH);

  int i = 0;
  while(i < MAX_LENGTH){
    char c = sysBuffer[i];
    if(c != '\0') 
      kernel->synchConsoleOut->PutChar(c);
    else 
      break;
    i++;
  }
  delete[] sysBuffer;
}



#endif /* ! __USERPROG_KSYSCALL_H__ */
