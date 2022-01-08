#include "pcb.h"
#include "utility.h"
#include "thread.h"
#include "addrspace.h"

void StartProcess_2(int id)
{
	printf("Vao duoc day r ne\n");
	char* fileName = kernel->pTab->getFileName(id);

	AddrSpace *space;
	space = new AddrSpace(fileName);
	printf("Vao duoc day r ne2\n");
	if(space == NULL)
	{
			printf("Vao duoc day r ne3\n");
		cerr << "PCB::Exec : Can't create AddSpace. \n";
		return;
	}
				printf("Vao duoc day r ne4n");
	kernel->currentThread->space = space;
			printf("Vao duoc day r ne5\n");
	space->InitRegisters();		
				printf("Vao duoc day r ne6\n");
	space->RestoreState();		
			printf("Vao duoc day r ne7\n");
	kernel->machine->Run();		
				printf("Vao duoc day r ne8\n");
	ASSERT(FALSE);		
}

PCB::PCB(int id)
{
    if (id == 0)
        this->parentID = -1;
    else
        this->parentID = kernel->currentThread->processID;

	this->numwait = this->exitcode = 0;
	this->thread = NULL;
    this->pid = id;

	this->joinsem = new Semaphore("joinsem",0);
	this->exitsem = new Semaphore("exitsem",0);
	this->multex = new Semaphore("multex",1);
	this->fileTable = new FileTable();
}

PCB::~PCB()
{
	if(joinsem != NULL)
		delete this->joinsem;
	if(exitsem != NULL)
		delete this->exitsem;
	if(multex != NULL)
		delete this->multex;
	if(thread != NULL)
	{		
		thread->freeSpace();
		thread->Finish();
	}
}

void PCB::SetFileName(char* fn)
{ 
	strcpy(FileName,fn);
}

char* PCB::GetFileName() 
{ 
	return this->FileName; 
}

int PCB::GetID()
{ 
	return pid; 
}

int PCB::GetNumWait() 
{ 
	return this->numwait; 
}

int PCB::GetExitCode() 
{ 
	return this->exitcode;
}

void PCB::SetExitCode(int ec)
{ 
	this->exitcode = ec; 
}

void PCB::JoinWait()
{
    joinsem->P();
}

void PCB::JoinRelease()
{ 
    joinsem->V();
}

void PCB::ExitWait()
{ 
    exitsem->P();
}

void PCB::ExitRelease() 
{
    exitsem->V();
}

void PCB::IncNumWait()
{
	multex->P();
	++numwait;
	multex->V();
}

void PCB::DecNumWait()
{
	multex->P();
	if(numwait > 0)
		--numwait;
	multex->V();
}


int PCB::Exec(char* filename, int id)
{  
	multex->P();

	this->thread = new Thread(filename);

	if(this->thread == NULL){
		printf("\nJhong du bo nho");
        multex->V();
		return -1;
	}
	printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");

	this->thread->processID = id;
	this->parentID = kernel->currentThread->processID;
 	this->thread->Fork((VoidFunctionPtr)StartProcess_2,(void*)id);
	// OpenFile* executable = kernel->fileSystem->Open(filename);
	// AddrSpace* space;

	// if(executable==NULL){
	// 	printf("Unable to open %s\n",filename);
	// }
	// else {
	// 	space = new AddrSpace(executable);
	// 	kernel->currentThread->space = space;
	// 	delete executable;
	// 	space->InitRegisters();
	// 	space->RestoreState();
	// 	kernel->machine->Run();
	// 	ASSERT(FALSE);
	// }

    multex->V();
	return id;

}

bool PCB::Create(char *name, int initialSize) {
   return fileTable->Create(name, initialSize);
}

int PCB::FindFreeSlot() {
   return fileTable->FindFreeSlot();
}

OpenFile* PCB::getOpenFileId(int id){
	return fileTable->getOpenFileId(id);
}

int PCB::Open(char* name){
	return fileTable->Open(name);
}
int PCB::Open(char* name, int type){
	return fileTable->Open(name, type);

}

void PCB::Close(int id){
	fileTable->closeFile(id);
}

char* PCB::getFileName(){
	return thread->getName();
};