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

void SysPrintNum(int number)
{
  int lastNum, index = 0, i = 0;
  char arr[100] = ""; //MAX_INT 2147483647
  if (number < 0){
    number*=-1;
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

int pow(int a, int b)
{
  int result = 1;
  for (int i = 0; i < b; i++)
  {
    result *= a;
  }
  return result;
}

bool isOutOfBound(char num[100], int length)
{
  int start = 0;
  char limit[11] = {'2', '1', '4', '7', '4', '8', '3', '6', '4', '7'};

  if (num[0] == '-')
  {
    if (length > 11)
      return true;
    if (length < 11)
      return false;
    start = 1;
    limit[9] = '8';
    for (int i = start; i < length; i++)
    {
      if (num[i] < limit[i - 1])
        return false;
      if (num[i] > limit[i - 1])
        return true;
    }
  }
  else
  {
    if (length > 10)
      return true;
    if (length < 10)
      return false;
    for (int i = start; i < length; i++)
    {
      if (num[i] < limit[i])
        return false;
      if (num[i] > limit[i])
        return true;
    }
  }
  return false;
}

bool isValidNumber(char num[100], int length)
{
  int start = 0;
  if (num[0] == '-')
    start = 1;
  for (int i = start; i < length; i++)
  {
    if (num[i] < '0' || num[i] > '9')
      return false;
  }
  return true;
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

int SysReadNum()
{
  char arr[100]; //MAX_INT 2147483647
  int result = 0, index = 0;
  int sign, start;
  char temp;
  do
  {
    temp = kernel->synchConsoleIn->GetChar();
    if (temp != EOF)
      arr[index++] = temp;
  } while (temp != EOF);
  if (isValidNumber(arr, index))
  {
    sign = arr[0] == '-' ? -1 : 1;
    start = sign == 1 ? 0 : 1;
    if (isOutOfBound(arr, index) && sign == -1)
      return -2147483647 - 1;
    if (isOutOfBound(arr, index) && sign == 1)
      return 2147483647;

    for (int i = index; i >= start; i--)
    {
      result = result * 10 + (arr[i] - '0');
    }
    return result * sign;
  }
  else
    result = 0;

  return result;
}

#endif /* ! __USERPROG_KSYSCALL_H__ */
