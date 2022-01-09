#ifndef FILETABLE_H
#define FILETABLE_H

#include "syscall.h"
#include "../filesys/openfile.h"

#define MAX_OPENFILE 10

class OpenFile;
typedef int OpenFileID;

// Process Control Block
class FileTable
{
private:
    OpenFile** fileTable;
public:
    FileTable();
    bool Create(char *name, int initialSize);
    int Open(char* name);
    int Open(char* name, int type);
    OpenFile* OpenF(char* name);
    OpenFile* OpenF(char* name, int type);
    OpenFile* getFileById(int id);
    void Close(int id);
    int FindFreeSlot();
    bool Remove(char* name);
    ~FileTable();
};

#endif // FILETABLE_H