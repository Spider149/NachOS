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

				case SC_Open:{
					//OpenFileId Open(char *name, int type)
					//name: Thanh ghi so 4
					//type: Thanh ghi so 5

					int nameAddr = kernel->machine->ReadRegister(4);
					int type = kernel->machine->ReadRegister(5);
					char* fileName;
					if (kernel->fileSystem->index > 10){
						kernel->machine->WriteRegister(2, -1);
						delete[] fileName;
						break;
					}
					fileName = User2System(nameAddr,256);
					if (type == 0 || type == 1){
						if (kernel->fileSystem->openFile[kernel->fileSystem->index] == kernel->fileSystem->Open(fileName, type)){
							cerr << "Open file " << fileName <<" success \n";
							kernel->machine->WriteRegister(2, kernel->fileSystem->index-1);
						} else {
							cerr << "Can not open file "<<fileName<<"\n";
							kernel->machine->WriteRegister(2, -1);
						}
					}
					else if (strcmp(fileName,"stdin") == 0){
						cerr <<"Stdin mode \n";
						kernel->machine->WriteRegister(2, 0);
					}
					else if (strcmp(fileName, "stdout") == 0){
						cerr <<"Stdout mode \n";
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
						if (kernel->fileSystem->openFile[nameAddr]){
							delete kernel->fileSystem->openFile[nameAddr];
							kernel->fileSystem->openFile[nameAddr] == NULL;
							kernel->machine->WriteRegister(2, 0);
							cerr<<"Close file success \n";
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
