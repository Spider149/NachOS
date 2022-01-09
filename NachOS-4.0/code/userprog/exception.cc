// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
#include "synchconsole.h"

#define MaxFileLength 32 // Do dai quy uoc cho file name

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	is in machine.h.
//----------------------------------------------------------------------


//c. Tang gia tri bien Program Counter (PC) len 4 de tiep tuc nap lenh

void increasePC(){
	//PrevPCReg <- PCReg
	int counter = kernel->machine->ReadRegister(PCReg);
	kernel->machine->WriteRegister(PrevPCReg, counter);
	//PCReg <- NextPCReg
	counter = kernel->machine->ReadRegister(NextPCReg);
	kernel->machine->WriteRegister(PCReg, counter);
	//NextPCReg <- PCReg + 4
	kernel->machine->WriteRegister(NextPCReg, counter + 4);
}

void
ExceptionHandler(ExceptionType which)
{
    int type = kernel->machine->ReadRegister(2);
	int result;

    DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

    switch (which) {
		// Everything ok!
		case NoException:
			return;

		// A program executed a system call.
		case SyscallException:
			switch(type) {
				case SC_Halt:
					DEBUG(dbgSys, "Shutdown, initiated by user program.\n");
					printf("\n\n Shutdown, initiated by user program.");
					SysHalt();

					ASSERTNOTREACHED();
					break;

				case SC_PrintNum:
					SysPrintNum((int)kernel->machine->ReadRegister(4));
					increasePC();
					return;
					ASSERTNOTREACHED();
					break;

				case SC_ReadNum:
					result = SysReadNum();
					kernel->machine->WriteRegister(2, (int)result);
					increasePC();
					return;
					ASSERTNOTREACHED();
					break;

				case SC_PrintChar:
					SysPrintChar((char)kernel->machine->ReadRegister(4));
					increasePC();
					return;
					ASSERTNOTREACHED();
					break;

				case SC_ReadChar:
					result = SysReadChar();
					kernel->machine->WriteRegister(2, (char)result);
					increasePC();
					return;
					ASSERTNOTREACHED();
					break;

				case SC_RandomNum:
					result = SysRandomNum();
					kernel->machine->WriteRegister(2, (int)result);
					increasePC();
					return;
					ASSERTNOTREACHED();
					break;

				case SC_PrintString:
					SysPrintString();
					increasePC();
					return;
					ASSERTNOTREACHED();
					break;

				case SC_ReadString:
					SysReadString();
					increasePC();
					return;
					ASSERTNOTREACHED();
					break;



				case SC_Add:
					DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");
		
					/* Process SysAdd Systemcall*/
		
					result = SysAdd(/* int op1 */(int)kernel->machine->ReadRegister(4),
							/* int op2 */(int)kernel->machine->ReadRegister(5));
					DEBUG(dbgSys, "Add returning with " << result << "\n");
					/* Prepare Result */
					kernel->machine->WriteRegister(2, (int)result);
					
					/* Modify return point */
					increasePC();

					return;
					ASSERTNOTREACHED();
					break;

				case SC_CreateFile: {
					int virtAddr;
					char* filename;
					DEBUG('a', "\n SC_CreateFile call ...");
					DEBUG('a', "\n Reading virtual address of filename");

					virtAddr = kernel->machine->ReadRegister(4); //Doc dia chi cua file tu thanh ghi R4
					DEBUG('a', "\n Reading filename.");
					
					//Sao chep khong gian bo nho User sang System, voi do dang toi da la (32 + 1) bytes
					filename = User2System(virtAddr, MaxFileLength + 1);
					if (strlen(filename) == 0)
					{
						printf("\n File name is not valid");
						DEBUG('a', "\n File name is not valid");
						kernel->machine->WriteRegister(2, -1); //Return -1 vao thanh ghi R2
						increasePC();
						return;
					}
					
					if (filename == NULL)  //Neu khong doc duoc
					{
						printf("\n Not enough memory in system");
						DEBUG('a', "\n Not enough memory in system");
						kernel->machine->WriteRegister(2, -1); //Return -1 vao thanh ghi R2
						delete filename;
						increasePC();
						return;
					}
					DEBUG('a', "\n Finish reading filename.");
					
					/*if (!kernel->fileSystem->Create(filename, 0)) //Tao file bang ham Create cua fileSystem, tra ve ket qua
					{
						//Tao file that bai
						printf("\n Error create file '%s'", filename);
						kernel->machine->WriteRegister(2, -1);
						delete filename;
						increasePC();
						return;
						//break;
					}*/

					if (!kernel->pTab->getPCB(kernel->currentThread->processID)->getFileTable()->Create(filename, 0)) //Tao file bang ham Create cua fileSystem, tra ve ket qua
					{
						//Tao file that bai
						printf("\n Error create file '%s'", filename);
						kernel->machine->WriteRegister(2, -1);
						delete filename;
						increasePC();
						return;
						//break;
					}
					
					//Tao file thanh cong
					kernel->machine->WriteRegister(2, 0);
					delete filename;
					increasePC(); //Day thanh ghi lui ve sau de tiep tuc ghi
					return;
				}
					break;

				case SC_Open:{
					//OpenFileId Open(char *name, int type)
					//name: Thanh ghi so 4
					//type: Thanh ghi so 5

					int nameAddr = kernel->machine->ReadRegister(4);
					int type = kernel->machine->ReadRegister(5);
					char* fileName;
					int freeSlot;
					fileName = User2System(nameAddr,256);

					freeSlot = kernel->pTab->getPCB(kernel->currentThread->processID)->getFileTable()->FindFreeSlot();
					if (freeSlot == -1){
						kernel->machine->WriteRegister(2, -1);
						delete[] fileName;
						increasePC();
						return;
					}
					if (type == 0 || type == 1){
						if (kernel->pTab->getPCB(kernel->currentThread->processID)->getFileTable()->Open(fileName, type) != -1){
							cerr << "Open file " << fileName <<" success \n";
							kernel->machine->WriteRegister(2, freeSlot);
						} 
					} else if (type == 2) {
							cerr << "Sdtin mode \n";
							kernel->machine->WriteRegister(2, 0);
					} else{
						cerr << "Stdout mode \n";
						kernel->machine->WriteRegister(2, 1);
					}
					if (fileName)
						delete[] fileName;

					/* Modify return point */
					increasePC();

					return;
		
					ASSERTNOTREACHED();
				}
					break;

				case SC_Close:{
					int nameAddr = kernel->machine->ReadRegister(4);
					int index = kernel->fileSystem->index;
					if (nameAddr >= 0 && nameAddr <= 9 && index >= nameAddr){
						if (kernel->pTab->getPCB(kernel->currentThread->processID)->getFileTable()->getOpenFileId(nameAddr)){
							kernel->pTab->getPCB(kernel->currentThread->processID)->getFileTable()->closeFile(nameAddr);;
							kernel->machine->WriteRegister(2, 0);
							cerr<<"Close file success \n";
							increasePC();
							return;
						}
					}
					cerr<<"Close file fail \n";
					kernel->machine->WriteRegister(2, -1);

					/* Modify return point */
					increasePC();

					return;
		
					ASSERTNOTREACHED();
				}
					break;
				
				case SC_Read:{
					int virtAddr = kernel->machine->ReadRegister(4); 
					int charcount = kernel->machine->ReadRegister(5); 
					int id = kernel->machine->ReadRegister(6);
					char* buffer;
					int newPos;
					int oldPos;

					if (id < 0 || id > 9) {
						printf("\nFile id nam ngoai bang mo ta file");
						kernel->machine->WriteRegister(2, -1);
						increasePC();
						return;
					}

					if (kernel->pTab->getPCB(kernel->currentThread->processID)->getFileTable()->getOpenFileId(id) == NULL)
					{
						printf("\nFile khong ton tai");
						kernel->machine->WriteRegister(2, -1);
						increasePC();
						return;
					}

					if (kernel->pTab->getPCB(kernel->currentThread->processID)->getFileTable()->getOpenFileId(id)->type == 3)
					{
						printf("\nFile stdout khong the doc");
						kernel->machine->WriteRegister(2, -1);
						increasePC();
						return;
					}

					oldPos = kernel->pTab->getPCB(kernel->currentThread->processID)->getFileTable()->getOpenFileId(id)->GetCurrentPos();
					buffer = User2System(virtAddr, charcount);

					if (kernel->pTab->getPCB(kernel->currentThread->processID)->getFileTable()->getOpenFileId(id)->type == 2)
					{
						int i = 0;
						while(i < charcount){
							char c = kernel->synchConsoleIn->GetChar();
							if(c=='\n' || c=='\0') break;
							buffer[i] = c;
							i++;
						}
						buffer[i]='\0';
						System2User(virtAddr, i+1, buffer);
						kernel->machine->WriteRegister(2, i+1);
						delete buffer;
						increasePC();
						return;
					} 

					if ((kernel->pTab->getPCB(kernel->currentThread->processID)->getFileTable()->getOpenFileId(id)->Read(buffer, charcount)) > 0)
					{
						newPos = kernel->pTab->getPCB(kernel->currentThread->processID)->getFileTable()->getOpenFileId(id)->GetCurrentPos();
						System2User(virtAddr, newPos - oldPos, buffer); 
						kernel->machine->WriteRegister(2, newPos - oldPos);
					}
					else kernel->machine->WriteRegister(2, -2);
					delete buffer;

					increasePC();
					return;
					ASSERTNOTREACHED();
				}
					break;

				case SC_Write:
				{
					int virtAddr = kernel->machine->ReadRegister(4); 
					int charcount =  kernel->machine->ReadRegister(5);
					int id =  kernel->machine->ReadRegister(6);
					int oldPos;
					int newPos;
					char *buffer;
					if (id < 0 || id > 9) 
					{
						printf("\nFile id nam ngoai bang mo ta file");
						kernel->machine->WriteRegister(2, -1);
						increasePC();
						return;
					}
					if (kernel->pTab->getPCB(kernel->currentThread->processID)->getFileTable()->getOpenFileId(id)== NULL)
					{
						printf("\nFile khong ton tai");
						kernel->machine->WriteRegister(2, -1);
						increasePC();
						return;
					}
					if (kernel->pTab->getPCB(kernel->currentThread->processID)->getFileTable()->getOpenFileId(id)->type == 1 || kernel->pTab->getPCB(kernel->currentThread->processID)->getFileTable()->getOpenFileId(id)->type == 2)
					{
						printf("\nKhong the ghi vao file chi doc va file stdin");
						kernel->machine->WriteRegister(2, -1);
						increasePC();
						return;
					}
					oldPos = kernel->pTab->getPCB(kernel->currentThread->processID)->getFileTable()->getOpenFileId(id)->GetCurrentPos();
					buffer = User2System(virtAddr, charcount);

					if (kernel->pTab->getPCB(kernel->currentThread->processID)->getFileTable()->getOpenFileId(id)->type == 0)
					{
						if ((kernel->pTab->getPCB(kernel->currentThread->processID)->getFileTable()->getOpenFileId(id)->Write(buffer, charcount)) > 0)
						{
							newPos = kernel->pTab->getPCB(kernel->currentThread->processID)->getFileTable()->getOpenFileId(id)->GetCurrentPos();
							kernel->machine->WriteRegister(2, newPos - oldPos);
							delete buffer;
							increasePC();
							return;
						}
					}
					if (kernel->pTab->getPCB(kernel->currentThread->processID)->getFileTable()->getOpenFileId(id)->type == 3)
					{
						int i = 0;
						while(i<charcount){
							if (buffer[i] != '\0' && buffer[i] != '\n') 
							{
								kernel->synchConsoleOut->PutChar(buffer[i]);
								i++;
							}
							else break;
						}
						kernel->machine->WriteRegister(2, i);
						delete buffer;
						increasePC();
						return;
					}
				}
				break;
				/* Tao cau truc du lieu de luu 10 semaphore
				* Tra ve 0 neu thanh cong, nguoc lai tra ve 1
				*/
				case SC_CreateSemaphore:{
					int virtAddr = kernel->machine->ReadRegister(4);
					int semVal = kernel->machine->ReadRegister(5);
					char* name = User2System(virtAddr, 33);
					if (name == NULL){
						DEBUG('a', "Not enough memory in system \n");
						printf("Not enough memory in system \n");
						kernel->machine->WriteRegister(2, -1);
						delete[] name;
						increasePC();
						return;
					}
					
					int res = kernel->semTab->Create(name, semVal);
					
					if (res == -1){
						DEBUG('a', "Khong the khoi tao semaphore \n");
						printf("Khong the khoi tao semaphore \n");
						kernel->machine->WriteRegister(2, -1);
						delete[] name;
						increasePC();
						return;
					}
					delete[] name;
					kernel->machine->WriteRegister(2, res);
					increasePC();
					return;
					ASSERTNOTREACHED();
				}
				break;

				/* Name: Ten cua Semaphore
				*  Tra ve 0 neu thanh cong, tra ve -1 neu loi
				*   Wait() -- release the lock, relinquish the CPU until signaled, 
				*		then re-acquire the lock
				*/
				case SC_Wait:{
					int virtAddr = kernel->machine->ReadRegister(4);
					char* name = User2System(virtAddr, 33);
					if (name == NULL){
						DEBUG('a', "Not enough memory in system \n");
						cerr << "Not enough memory in system \n";
						kernel->machine->WriteRegister(2, -1);
						delete[] name;
						increasePC();
						return;
					}

					int res = kernel->semTab->Wait(name);

					if (res == -1){
						DEBUG('a', "Khong ton tai semaphore nay \n");
						cerr << "Khong ton tai semaphore nay \n";
						kernel->machine->WriteRegister(2, -1);
						delete[] name;
						increasePC();
						return;
					}
					delete[] name;
					kernel->machine->WriteRegister(2, res);
					increasePC();
					return;
					ASSERTNOTREACHED();
				}
				break;

				/* Name: Ten cua Semaphore
				* Signal() -- wake up a thread, if there are any waiting on 
				*		the condition
				*  Tra ve 0 neu thanh cong, tra ve -1 neu loi
				*/
				case SC_Signal:{
					int virtAddr = kernel->machine->ReadRegister(4);
					char* name = User2System(virtAddr, 33);
					if (name == NULL){
						DEBUG('a', "Not enough memory in system \n");
						cerr << "Not enough memory in system \n";
						kernel->machine->WriteRegister(2, -1);
						delete[] name;
						increasePC();
						return;
					}

					int res = kernel->semTab->Signal(name);

					if (res == -1){
						DEBUG('a', "Khong ton tai semaphore nay \n");
						cerr << "Khong ton tai semaphore nay \n";
						kernel->machine->WriteRegister(2, 0);
						delete[] name;
						increasePC();
						return;
					}
					delete[] name;
					kernel->machine->WriteRegister(2, res);
					increasePC();
					return;
					ASSERTNOTREACHED();
				}
				break;

				case SC_Exec:
				{
					// Input: vi tri int
					// Output: Fail return -1, Success: return id cua thread dang chay
					// SpaceId Exec(char *name);
					int virtAddr;
					virtAddr = kernel->machine->ReadRegister(4);	// doc dia chi ten chuong trinh tu thanh ghi r4
					char* name;
					name = User2System(virtAddr, MaxFileLength + 1); // Lay ten chuong trinh, nap vao kernel
			
					if(name == NULL)
					{
						DEBUG('a', "\n Not enough memory in System");
						printf("\n Not enough memory in System");
						kernel->machine->WriteRegister(2, -1);
						increasePC();
						return;
					}
				
					if (kernel->pTab->getPCB(kernel->currentThread->processID)->getFileTable()->Open(name) == -1)
					{
						printf("\nExec:: Can't open this file.");
						kernel->machine->WriteRegister(2,-1);
						increasePC();
						return;
					}

					// Return child process id
					int id = kernel->pTab->ExecUpdate(name); 
					kernel->machine->WriteRegister(2,id);

					delete[] name;	
					increasePC();
					return;
				}
				break;

				case SC_Join:{
					int id = kernel->machine->ReadRegister(4);
					int res = kernel->pTab->JoinUpdate(id);

					kernel->machine->WriteRegister(2, res);
					increasePC();
					return;
					ASSERTNOTREACHED();
				}
				break;

				case SC_Exit:{
					int status = kernel->machine->ReadRegister(4);

					if(status != 0){
						increasePC();
						return;
					}
					int res = kernel->pTab->ExitUpdate(status);

					kernel->currentThread->freeSpace();
					kernel->currentThread->Finish();
					increasePC();
					return;
					ASSERTNOTREACHED();
				}
				break;
				

				case SC_GetPID:
					result = kernel->currentThread->processID;
					kernel->machine->WriteRegister(2, (int)result);
					increasePC();
					return;
					ASSERTNOTREACHED();
					break;


				default:
					cerr << "Unexpected system call " << type << "\n";
					break;
			}
			break;

		

		// No valid translation found
		case PageFaultException:
			DEBUG(dbgAddr, "No valid translation found \n");
			cerr << "No valid translation found \n";
			SysHalt();
			break;

		// Write attempted to page marked "read-only"
		case ReadOnlyException:
			DEBUG(dbgAddr, "Write attempted to page marked read-only \n");
			cerr << "Write attempted to page marked read-only \n";
			SysHalt();
			break;

		// Translation resulted in an invalid physical address
		case BusErrorException:
			DEBUG (dbgAddr, "Translation resulted in an invalid physical address \n");
			cerr << "Translation resulted in an invalid physical address \n";
			SysHalt();
			break;

		// Unaligned reference or one that was beyond the end of the address space
		case AddressErrorException:
			DEBUG (dbgAddr, "Unaligned reference or one that was beyond the end of the address space \n");
			cerr << "Unaligned reference or one that was beyond the end of the address space \n";
			SysHalt();
			break;
		
		// Integer overflow in add or sub.
		case OverflowException:
			DEBUG (dbgAddr, "Unaligned reference or one that was beyond the end of the address space \n");
			cerr << "Unaligned reference or one that was beyond the end of the address space \n";
			SysHalt();
			break;

		// Unimplemented or reserved instr.
		case IllegalInstrException:
			DEBUG (dbgAddr, "Unimplemented or reserved instr \n");
			cerr << "Unimplemented or reserved instr \n";
			SysHalt();
			break;

		case NumExceptionTypes:
			DEBUG (dbgAddr, "\n Num Exception Types");
			cerr << "Num Exception Types \n";
			SysHalt();
			break;

		default:
			cerr << "Unexpected user mode exception" << (int)which << "\n";
			break;
    }
    ASSERTNOTREACHED();
}