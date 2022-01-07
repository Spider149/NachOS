#ifndef PTABLE_H
#define PTABLE_H

#include "bitmap.h"
#include "pcb.h"
#include "synch.h"

#define MAX_PROCESS 10

class PTable
{
private:
	int psize;
	Bitmap *bm;                
	PCB* pcb[MAX_PROCESS];
	Semaphore* bmsem;           

public:
    PTable(int size);                                      
    ~PTable();                 

    int ExecUpdate(char*);      // Xử lý cho system call SC_Exit
    int ExitUpdate(int);        // Xử lý cho system call SC_Exit
    int JoinUpdate(int);        // Xử lý cho system call SC_Join

    int GetFreeSlot();          // tìm free slot để lưu thông tin cho tiến trình mới
    bool IsExist(int pid);      // kiểm tra tồn tại processID này không?

    void Remove(int pid);       // khi tiến trình kết thúc, delete processID ra khỏi mảng quản lý nó
};
#endif // PTABLE_H