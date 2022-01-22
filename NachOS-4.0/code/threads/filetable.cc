#include "filetable.h"
#include "stdio.h"
#include "utility.h"
#include "sysdep.h"
#include "copyright.h"

//Khoi tao mang gom 10 OpenFile* quan ly cac file dang mo cua mot tien trinh
FileTable::FileTable(){
    fileTable = new OpenFile*[MAX_OPENFILE];
    for (int i = 0; i < MAX_OPENFILE; ++i)
        fileTable[i] = NULL;
    this->Create("stdin", 0);
    this->Create("stdout", 0);
    fileTable[0] = this->OpenF("stdin", 2);
    fileTable[1] = this->OpenF("stdout", 3);
}

//Tao mot file bang UNIX
bool FileTable::Create(char *name, int initialSize) {
    int fileDescriptor = OpenForWrite(name);

    if (fileDescriptor == -1) return FALSE;
    Close(fileDescriptor); 
    return TRUE; 
}

//Tim kiem file dang mo bang id
OpenFile* FileTable::getFileById(int id){
    return fileTable[id];
}

//Mo file  bang ten file va co the mo hai loai khac nhau: file chi doc, file co the doc va ghi
int FileTable::Open(char* name, int type){
    int fileDescriptor;
    if(type == 3 || type == 0)
        fileDescriptor = OpenForRead(name, FALSE);
    else if (type == 2)
        fileDescriptor = OpenForWrite(name);
    else if (type == 1)
        fileDescriptor = OpenForReadWrite(name, FALSE);

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


//Mo file bang ten file
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


//Mo mot file gom ten bang UNIX
OpenFile* FileTable::OpenF(char* name){
    int fileDescriptor = OpenForReadWrite(name, FALSE);
    if (fileDescriptor == -1) return NULL;
    return new OpenFile(fileDescriptor);
}

//Mo mot file gom ten va loai bang UNIX
OpenFile* FileTable::OpenF(char* name, int type){
    int fileDescriptor;
    if(type == 3 || type == 0)
        fileDescriptor = OpenForRead(name, FALSE);
    else if (type == 2)
        fileDescriptor = OpenForWrite(name);
    else if (type == 1)
        fileDescriptor = OpenForReadWrite(name, FALSE);
    if (fileDescriptor == -1) return NULL;
    return new OpenFile(fileDescriptor);
}

//Tim slot trong
int FileTable::FindFreeSlot(){
    for(int i = 2; i < MAX_OPENFILE; i++)
		if(fileTable[i] == NULL)
            return i;
    printf("No free slot on file table \n");
	return -1;
}

//Loai bo file dang mo bang ten bang UNIX 
bool FileTable::Remove(char* name){
    return Unlink(name) == 0; 
}


//Dong file dang mo
void FileTable::Close(int id){
    if (fileTable[id] != NULL){
        delete fileTable[id];
        fileTable[id] = NULL;
    }
}

//Ham huy filetable
FileTable::~FileTable(){
    for (int i = 0; i < MAX_OPENFILE; ++i)
        if (fileTable[i] != NULL)
            delete fileTable[i];
    delete[] fileTable;
}