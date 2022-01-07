#ifndef STABLE_H
#define STABLE_H
#define MAX_SEMAPHORE 10

#include "synch.h"
#include "sem.h"
#include "bitmap.h"

class Sem;

class STable {
  private:
    Bitmap* bm;
    Sem* semTab[MAX_SEMAPHORE];
    // quan ly slot trong
    // quan ly toi da 10 doi tuong Semaphore
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