#ifndef FILETABLE_H
#define FILETABLE_H
#define MAX_FILE 10

#include "synch.h"

class STable {
  private:
    int fileTabe[MAX_FILE];
    // quan ly slot trong
    // quan ly toi da 10 doi tuong Sem
    
    int FindByName(char* name);
    
  public:
    STable();

    ~STable();

    int Create(char* name, int init);

    int Wait(char* name);
    
    int Signal(char* name);
    
    int FindFreeSlot();
};


#endif