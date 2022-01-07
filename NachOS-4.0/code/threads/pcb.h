#ifndef PCB_H
#define PCB_H

#include "thread.h"
#include "synch.h"
#include "filetable.h"

class Semaphore;

// Process Control Block
class PCB
{
private:
    Semaphore* joinsem;         // semaphore cho quá trình join
    Semaphore* exitsem;         // semaphore cho quá trình exit
    Semaphore* multex;          // semaphore cho quá trình truy xuất đọc quyền  

    int pid;
    int exitcode;		
    int numwait;                // số tiến trình đã join
    char FileName[64];          // Ten cua tien trinh
    Thread* thread;             // Tien trinh cua chuong trinh

    FileTable* fileTable;
public:
    int parentID;               // ID cua tien trinh cha

    PCB(int id);           
    ~PCB();                 

    int Exec(char* filename,int pid);        // Tao mot thread moi
    int GetID();                // Trả về ProcessID của tiến trình gọi thực hiện
    int GetNumWait();           // Trả về số lượng tiến trình chờ


    void JoinWait();            // 1. Tiến trình cha đợi tiến trình con kết thúc
    void ExitWait();             // 4. Tiến trình con kết thúc

    void JoinRelease();         // 2. Báo cho tiến trình cha thực thi tiếp
    void ExitRelease();         // 3. Cho phép tiến trình con kết thúc

    void IncNumWait();          // Tăng số tiến trình chờ
    void DecNumWait();          // Giảm số tiến trình chờ

    void SetExitCode(int ec);      // Đặt exitcode của tiến trình
    int GetExitCode();          // Trả về exitcode

    void SetFileName(char* fn);    // Set ten tien trinh
    char* GetFileName();        // Tra ve ten tien trinh

    bool Create(char *name, int initialSize);
    int FindFreeSlot();
    OpenFile* getOpenFileId(int id);
    OpenFile* Open(char* name);
    OpenFile* Open(char* name, int type);
    void Close(int id);
    char* getFileName();
};

#endif // PCB_H