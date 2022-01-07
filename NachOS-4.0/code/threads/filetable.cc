#include "filetable.h"
#include "stdio.h"
#include "utility.h"
    
FileTable::FileTable(){
    fileTable = new OpenFile*[10];
    index = 0;
    for (int i = 0; i < 10; ++i)
        fileTable[i] = NULL;
    this->Create("stdin", 0);
    this->Create("stdout", 0);
    fileTable[index++] = this->Open("stdin", 2);
    fileTable[index++] = this->Open("stdout", 3);
}

bool FileTable::Create(char *name, int initialSize) {
    int fileDescriptor = OpenForWrite(name);

    if (fileDescriptor == -1) return FALSE;
    Close(fileDescriptor); 
    return TRUE; 
}

OpenFile* FileTable::Open(char* name, int type){
    int fileDescriptor = OpenForReadWrite(name, FALSE);

	  if (fileDescriptor == -1) return NULL;
	  //index++;
	  return new OpenFile(fileDescriptor, type);
}

OpenFile* FileTable::Open(char *name) {
    int fileDescriptor = OpenForReadWrite(name, FALSE);

    if (fileDescriptor == -1) return NULL;
    //index++;
    int index;
    bool Create(char *name, int initialSize);
    return new OpenFile(fileDescriptor);
}
int FileTable::FindFreeSlot(){
    for(int i = 2; i < 10; i++)
	{
		if(fileTable[i] == NULL) return i;
	}
	return -1;
}
bool FileTable::Remove(char* name){
    return Unlink(name) == 0; 
}

int FileTable::ReadAt(char* buffer, int charcount, OpenFileID id){

}
    
int FileTable::WriteAt(char* buffer, int charcount, OpenFileID id){

}
    
FileTable::~FileTable(){
    for (int i = 0; i < 10; ++i)
			if (fileTable[i] != NULL)
				delete fileTable[i];
		delete[] fileTable;
}