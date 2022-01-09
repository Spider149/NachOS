#include "filetable.h"
#include "stdio.h"
#include "utility.h"
#include "sysdep.h"
#include "copyright.h"
    
FileTable::FileTable(){
    fileTable = new OpenFile*[MAX_OPENFILE];
    for (int i = 0; i < MAX_OPENFILE; ++i)
        fileTable[i] = NULL;
    this->Create("stdin", 0);
    this->Create("stdout", 0);
    fileTable[0] = this->OpenF("stdin", 2);
    fileTable[1] = this->OpenF("stdout", 3);
}

bool FileTable::Create(char *name, int initialSize) {
    int fileDescriptor = OpenForWrite(name);

    if (fileDescriptor == -1) return FALSE;
    Close(fileDescriptor); 
    return TRUE; 
}

OpenFile* FileTable::getFileById(int id){
    return fileTable[id];
}

int FileTable::Open(char* name, int type){
    int fileDescriptor = OpenForReadWrite(name, FALSE);

	if (fileDescriptor == -1) return -1;
	int freeSlot = FindFreeSlot();
    if (freeSlot < 0) {
        return -1;
    }
    fileTable[freeSlot] = new OpenFile(fileDescriptor, type);
    if (fileTable[freeSlot] != NULL)
        return freeSlot;
    return -1;
}

int FileTable::Open(char *name) {
    int fileDescriptor = OpenForReadWrite(name, FALSE);

    if (fileDescriptor == -1) {
        return -1;
    }
    int freeSlot = FindFreeSlot();
    if (freeSlot < 0) {
        return -1;
    }
    fileTable[freeSlot] = new OpenFile(fileDescriptor);
    if (fileTable[freeSlot] != NULL)
        return freeSlot;
    return -1;
}

OpenFile* FileTable::OpenF(char* name){
    int fileDescriptor = OpenForReadWrite(name, FALSE);
    if (fileDescriptor == -1) return NULL;
    return new OpenFile(fileDescriptor);
}
OpenFile* FileTable::OpenF(char* name, int type){
    int fileDescriptor = OpenForReadWrite(name, FALSE);
    if (fileDescriptor == -1) return NULL;
    return new OpenFile(fileDescriptor);
}
int FileTable::FindFreeSlot(){
    for(int i = 2; i < MAX_OPENFILE; i++)
		if(fileTable[i] == NULL)
            return i;
    printf("No free slot on file table \n");
	return -1;
}
bool FileTable::Remove(char* name){
    return Unlink(name) == 0; 
}

void FileTable::Close(int id){
    if (fileTable[id] != NULL){
        delete fileTable[id];
        fileTable[id] = NULL;
    }
}
    
FileTable::~FileTable(){
    for (int i = 0; i < MAX_OPENFILE; ++i)
        if (fileTable[i] != NULL)
            delete fileTable[i];
    delete[] fileTable;
}