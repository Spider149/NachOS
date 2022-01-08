#ifndef FILETABLE_H
#define FILETABLE_H

#include "syscall.h"

class OpenFile;
typedef int OpenFileID;

// Process Control Block
class FileTable
{
private:
    OpenFile** fileTable;
    int index;
public:                
    FileTable();
    bool Create(char *name, int initialSize);
    int Open(char* name);
    int Open(char* name, int type);
    OpenFile* OpenF(char* name);
    OpenFile* OpenF(char* name, int type);
    OpenFile* getOpenFileId(int id);
    void closeFile(int id);
    int FindFreeSlot();
    bool Remove(char* name);
    ~FileTable();
};

#endif // FILETABLE_H