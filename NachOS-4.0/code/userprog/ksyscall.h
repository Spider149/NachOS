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

#include "kernel.h"
#include "synchconsole.h"




void SysHalt()
{
  kernel->interrupt->Halt();
}


int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

void SysPrintNum(int number){
  int lastNum,index= 0,i=0;
  char arr[20]="";
  do{
    lastNum=number%10;
    arr[index++]=lastNum;
    number=number/10;
  }
  while(number!=0);
  while(i<index){
    kernel->synchConsoleOut->PutChar(arr[index-1-i]+'0');
    i++;
  }
}





#endif /* ! __USERPROG_KSYSCALL_H__ */
