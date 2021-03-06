#include "ptable.h"
#include "kernel.h"
#include "openfile.h"

PTable::PTable(int size)
{
    if (size < 0)
        return;

    psize = size;
    bm = new Bitmap(size);
    bmsem = new Semaphore("bmsem",1);

    for(int i=0;i<MAX_PROCESS;i++){
        pcb[i]=0;
    }
	// tao pcb dau tien trong ptable
	bm->Mark(0);

	pcb[0] = new PCB(0);
	pcb[0]->parentID = -1;
	pcb[0]->SetFileName("./scheduler");
}

PTable::~PTable()
{
    if( bm != 0 )
	delete bm;

    for(int i=0;i<psize;i++) {
        if(pcb[i] != 0) delete pcb[i];
    }

	if( bmsem != 0)
		delete bmsem;
}

int PTable::ExecUpdate(char* name)
{
	bmsem->P();

	//kiem tra xem ten tien trinh co null khong
	if(name == NULL)
	{
		printf("\nPTable::Exec : Can't not execute name is NULL.\n");
		bmsem->V();
		return -1;
	}
	// kiem tra xem tien trinh co goi lai chinh no hay khong
	if( strcmp(name,"./test/scheduler") == 0 || strcmp(name,kernel->currentThread->getName()) == 0 )
	{
		printf("\nPTable::Exec : Can't not execute itself.\n");		
		bmsem->V();
		return -1;
	}

	// Tìm slot trống trong bảng Ptable.
	int index = this->GetFreeSlot();


	if(index < 0)
	{	
		// khong tim duoc slot
		printf("\nPTable::Exec :There is no free slot.\n");
		bmsem->V();
		return -1;
	}

	// tao mot pcb moi
	pcb[index] = new PCB(index);
	pcb[index]->SetFileName(name);
    pcb[index]->parentID = kernel->currentThread->processID;


	// Gọi thực thi phương thức Exec của lớp PCB.
	int pid = pcb[index]->Exec(name,index);

	bmsem->V();
	return pid;
}

int PTable::JoinUpdate(int id)
{
	// Ta kiểm tra tính hợp lệ của processID id và kiểm tra tiến trình gọi Join có phải là cha của tiến trình
	// có processID là id hay không. Nếu không thỏa, ta báo lỗi hợp lý và trả về -1.
	if(id < 0)
	{
		printf("\nPTable::JoinUpdate : id = %d", id);
		return -1;
	}

	if(kernel->currentThread->processID != pcb[id]->parentID)
	{
		printf("\nPTable::JoinUpdate Can't join in process which is not it's parent process.\n");
		return -1;
	}

    // Tăng numwait và gọi JoinWait() để chờ tiến trình con thực hiện.
	// Sau khi tiến trình con thực hiện xong, tiến trình đã được giải phóng
	pcb[pcb[id]->parentID]->IncNumWait();

	pcb[id]->JoinWait();

	// Xử lý exitcode.	
	int ec = pcb[id]->GetExitCode();
    // ExitRelease() để cho phép tiến trình con thoát.
	pcb[id]->ExitRelease();

    // Successfully
	return ec;
}
int PTable::ExitUpdate(int exitcode)
{              
    // Nếu tiến trình gọi là main process thì gọi Halt().
	int id = kernel->currentThread->processID;
	if(id == 0)
	{
		kernel->currentThread->freeSpace();		
		kernel->interrupt->Halt();
		return 0;
	}

	// kiem tra tien trinh co ton tai khong
    if(IsExist(id) == false)
	{
		printf("\nPTable::ExitUpdate: This %d is not exist. Try again?", id);
		return -1;
	}


	// Ngược lại gọi SetExitCode để đặt exitcode cho tiến trình gọi.
	pcb[id]->SetExitCode(exitcode);
	pcb[pcb[id]->parentID]->DecNumWait();

    // Gọi JoinRelease để giải phóng tiến trình cha đang đợi nó(nếu có) và ExitWait() để xin tiến trình cha
    // cho phép thoát.
	pcb[id]->JoinRelease();
	pcb[id]->ExitWait();

	Remove(id);
	return exitcode;
}

// Find free slot in order to save the new process infom
int PTable::GetFreeSlot()
{
	return bm->FindAndSet();
}

// Check if Process ID is Exist
bool PTable::IsExist(int pid)
{
	return bm->Test(pid);
}

// Remove proccess ID out of table
// When it ends
void PTable::Remove(int pid)
{
	bm->Clear(pid);
	if(pcb[pid] != 0)
		delete pcb[pid];
}

char* PTable::getFileName(int id){
	return pcb[id]->GetFileName();
}



