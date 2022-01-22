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

				//int CreateFile(char* name)
				case SC_CreateFile: {
					int virtAddr;
					char* filename;
					DEBUG('a', "\n SC_CreateFile call ...");
					DEBUG('a', "\n Reading virtual address of filename");

					virtAddr = kernel->machine->ReadRegister(4); // Đọc địa chỉ của file từ thanh ghi R4
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

					if (!kernel->pTable->getPCB(kernel->currentThread->processID)->getFileTable()->Create(filename, 0)) //Tao file bang ham Create cua fileTable, tra ve ket qua
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
					ASSERTNOTREACHED();
					break;
				}

				//OpenFileId Open(char *name, int type)
				case SC_Open: {
					//Dia chi cua name: Thanh ghi so 4
					int nameAddr = kernel->machine->ReadRegister(4);
					//type: Thanh ghi so 5
					int type = kernel->machine->ReadRegister(5);
					char* fileName;
					int freeSlot;
					fileName = User2System(nameAddr,256);

					//Tim slot trong trong bang mo ta file fileTable
					freeSlot = kernel->pTable->getPCB(kernel->currentThread->processID)->getFileTable()->FindFreeSlot();
					//Neu khong co slot trong
					if (freeSlot == -1){
						//That bai tra ve -1
						kernel->machine->WriteRegister(2, -1);
						delete[] fileName;
						increasePC();
						return;
					}
					if (type == 0 || type == 1 || type == 2 || type == 3){
						//Mo file bang phuong thuc Open cua fileTable
						int fid = kernel->pTable->getPCB(kernel->currentThread->processID)->getFileTable()->Open(fileName, type);
						if ( fid != -1){
							//Xu ly khi loai file duoc mo la: 0: file chi doc, 1: file doc va ghi
							if (type == 0 || type == 1){
								//cerr << "Open file " << fileName <<" success \n";
								//Thanh cong tra ve OpenFileId
								kernel->machine->WriteRegister(2, fid);
							} else  if (type == 2){ //Xu ly loai file la 2: stdin mode
								cerr << "Sdtin mode \n";
								kernel->machine->WriteRegister(2, 0);
							} else { //Xy ly loai file la 3: stdout mode
								cerr << "Stdout mode \n";
								kernel->machine->WriteRegister(2, 1);
							}
						} else{
							//That bai tra ve -1
							kernel->machine->WriteRegister(2, -1);
						}
					}
					if (fileName)
						delete[] fileName;

					/* Modify return point */
					increasePC();
					return;
					ASSERTNOTREACHED();
					break;
				}

				//int Close(OpenFileId id)
				case SC_Close: {
					//id cua file: Thanh ghi so 4
					int nameAddr = kernel->machine->ReadRegister(4);
					//Chi xu ly khi id cua file nam trong [0, 9]
					if (nameAddr >= 0 && nameAddr <= 9){
						//Neu mo file thanh cong
						if (kernel->pTable->getPCB(kernel->currentThread->processID)->getFileTable()->getFileById(nameAddr)){
							//Goi ham xoa file cua fileTable
							kernel->pTable->getPCB(kernel->currentThread->processID)->getFileTable()->Close(nameAddr);
							//Thanh cong tra ve 0
							kernel->machine->WriteRegister(2, 0);
							//cerr<<"Close file success \n";
							increasePC();
							return;
						}
					}

					//Khong thanh cong tra ve -1
					cerr<<"Close file fail \n";
					kernel->machine->WriteRegister(2, -1);

					/* Modify return point */
					increasePC();
					return;
					ASSERTNOTREACHED();
					break;
				}
				
				//int Read(char* buffer, int size, OpenFileId id)
				case SC_Read:{
					int virtAddr = kernel->machine->ReadRegister(4); 
					int charcount = kernel->machine->ReadRegister(5); 
					int id = kernel->machine->ReadRegister(6);
					char* buffer;
					int newPos;
					int oldPos;

					// kiem tra index cua file co hop le khong
					if (id < 0 || id > 9) {
						printf("\nFile id nam ngoai bang mo ta file");
						kernel->machine->WriteRegister(2, -1);
						increasePC();
						return;
					}

					// kiem tra xem file co ton tai khong
					if (kernel->pTable->getPCB(kernel->currentThread->processID)->getFileTable()->getFileById(id) == NULL)
					{
						printf("\nFile khong ton tai");
						kernel->machine->WriteRegister(2, -1);
						increasePC();
						return;
					}

					// neu la file stdout thi khong cho doc
					if (kernel->pTable->getPCB(kernel->currentThread->processID)->getFileTable()->getFileById(id)->type == 3)
					{
						printf("\nFile stdout khong the doc");
						kernel->machine->WriteRegister(2, -1);
						increasePC();
						return;
					}


					//lay vi tri hien tai cua con tro file
					oldPos = kernel->pTable->getPCB(kernel->currentThread->processID)->getFileTable()->getFileById(id)->GetCurrentPos();
					buffer = User2System(virtAddr, charcount);

					// neu la file stdin, tuc la doc tu ban phim, dung synchconsolein
					if (kernel->pTable->getPCB(kernel->currentThread->processID)->getFileTable()->getFileById(id)->type == 2)
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

					// doc binh thuong, lay vi tri con tro sau khi doc de tinh ra so ky tu doc duoc thuc su

					if ((kernel->pTable->getPCB(kernel->currentThread->processID)->getFileTable()->getFileById(id)->Read(buffer, charcount)) > 0)
					{
						newPos = kernel->pTable->getPCB(kernel->currentThread->processID)->getFileTable()->getFileById(id)->GetCurrentPos();
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

				//int Write(char* buffer, int size, OpenFileId id)
				case SC_Write: {
					int virtAddr = kernel->machine->ReadRegister(4); 
					int charcount =  kernel->machine->ReadRegister(5);
					int id =  kernel->machine->ReadRegister(6);
					int oldPos;
					int newPos;
					char *buffer;
					// kiem tra xem index file co hop le khong
					if (id < 0 || id > 9) 
					{
						printf("\nFile id nam ngoai bang mo ta file");
						kernel->machine->WriteRegister(2, -1);
						increasePC();
						return;
					}
					// kiem tra xem file co ton tai khong
					if (kernel->pTable->getPCB(kernel->currentThread->processID)->getFileTable()->getFileById(id)== NULL)
					{
						printf("\nFile khong ton tai");
						kernel->machine->WriteRegister(2, -1);
						increasePC();
						return;
					}
					// khong cho phep ghi vao file chi doc va stdin
					if (kernel->pTable->getPCB(kernel->currentThread->processID)->getFileTable()->getFileById(id)->type == 1 || kernel->pTable->getPCB(kernel->currentThread->processID)->getFileTable()->getFileById(id)->type == 2)
					{
						printf("\nKhong the ghi vao file chi doc va file stdin");
						kernel->machine->WriteRegister(2, -1);
						increasePC();
						return;
					}
					// lay vi tri hien tai cua con tro trong file
					oldPos = kernel->pTable->getPCB(kernel->currentThread->processID)->getFileTable()->getFileById(id)->GetCurrentPos();
					// chuyen tu vung nho user sang system
					buffer = User2System(virtAddr, charcount);

					//neu la file doc va ghi
					if (kernel->pTable->getPCB(kernel->currentThread->processID)->getFileTable()->getFileById(id)->type == 0)
					{
						if ((kernel->pTable->getPCB(kernel->currentThread->processID)->getFileTable()->getFileById(id)->Write(buffer, charcount)) > 0)
						{
							//lay vi tri moi cua con tro sau khi doc de tinh so ky tu doc duoc thuc su
							newPos = kernel->pTable->getPCB(kernel->currentThread->processID)->getFileTable()->getFileById(id)->GetCurrentPos();
							kernel->machine->WriteRegister(2, newPos - oldPos);
							delete buffer;
							increasePC();
							return;
						}
					}
					// neu ghi vao file stdout, thi dung synchconsoleout de xuat cac ky tu ra man hinh
					if (kernel->pTable->getPCB(kernel->currentThread->processID)->getFileTable()->getFileById(id)->type == 3)
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
					break;
				}

				//SpaceId Exec(char* exec_name)
				case SC_Exec: {
					// Input: vi tri int
					// Output: Fail return -1, Success: return id cua thread dang chay
					// SpaceId Exec(char *name);
					int virtAddr;
					virtAddr = kernel->machine->ReadRegister(4);	
					char* name;

					//lay ten chuong trinh nap vao kernel
					name = User2System(virtAddr, MaxFileLength + 1); 
			
					if(name == NULL)
					{
						DEBUG('a', "\n Not enough memory in System");
						printf("\n Not enough memory in System");
						kernel->machine->WriteRegister(2, -1);
						increasePC();
						return;
					}

					// kiem tra xem file nay co ton tai trong he thong khong
					OpenFile* tmp = kernel->pTable->getPCB(kernel->currentThread->processID)->getFileTable()->OpenF(name);
					if (tmp == NULL)
					{
						printf("\nExec:: Can't open this file.");
						kernel->machine->WriteRegister(2,-1);
						increasePC();
						return;
					}
					tmp->~OpenFile();

					
					int id = kernel->pTable->ExecUpdate(name); 
					kernel->machine->WriteRegister(2,id);

					delete[] name;	
					increasePC();
					return;
				}
				break;

				//int Join(SpaceID id)
				case SC_Join: {
					//Doc id tu thanh ghi 4
					int id = kernel->machine->ReadRegister(4);
					
					//Goi ham JoinUpdate cua PTable
					int res = kernel->pTable->JoinUpdate(id);

					//Tra lai ket qua la res
					kernel->machine->WriteRegister(2, res);
					increasePC();
					return;
					ASSERTNOTREACHED();
				}
				break;

				//void Exit(int status)
				case SC_Exit: {
					//Doc status tu thanh ghi 4
					int status = kernel->machine->ReadRegister(4);

					//Khong la exitcode
					if(status != 0){
						increasePC();
						return;
					}

					//Goi ham ExitUpdate cua PTable
					int res = kernel->pTable->ExitUpdate(status);

					//Giai phong vung nho
					kernel->currentThread->freeSpace();

					//Ket thuc tien trinh
					kernel->currentThread->Finish();
					increasePC();
					return;
					ASSERTNOTREACHED();
				}
				break;
				
				//int CreateSemaphore(char* name, int semVal)
				case SC_CreateSemaphore: {
					//Doc dia chi cua ten cua semaphore tu thanh ghi 4
					int virtAddr = kernel->machine->ReadRegister(4);
					//Doc gia tri semVal tu thanh ghi 5
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
					
					int res = kernel->sTable->Create(name, semVal);
					
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
					break;
				}

				//int Wait(char* name)
				case SC_Wait: {
					//Doc dia chi cua ten cua semaphore tu thanh ghi 4
					int virtAddr = kernel->machine->ReadRegister(4);
					char* name = User2System(virtAddr, 33);
					//Neu ten bang  NULL
					if (name == NULL){
						DEBUG('a', "Not enough memory in system \n");
						cerr << "Not enough memory in system \n";
						kernel->machine->WriteRegister(2, -1);
						delete[] name;
						increasePC();
						return;
					}

					//Goi ham Wait cua STable
					int res = kernel->sTable->Wait(name);

					//Neu Wait that bai
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
					break;
				}

				//int Signal(char* name)
				case SC_Signal: {
					//Doc dia chi cua ten cua semaphore tu thanh ghi 4
					int virtAddr = kernel->machine->ReadRegister(4);
					char* name = User2System(virtAddr, 33);
					//Neu ten bang  NULL
					if (name == NULL){
						DEBUG('a', "Not enough memory in system \n");
						cerr << "Not enough memory in system \n";
						kernel->machine->WriteRegister(2, -1);
						delete[] name;
						increasePC();
						return;
					}

					//Goi ham Signal cua STable
					int res = kernel->sTable->Signal(name);

					//Neu Signal that bai
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
					break;
				}

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