#ifndef STABLE_H
#define STABLE_H
#define MAX_SEMAPHORE 10

#include "synch.h"
#include "sem.h"
#include "bitmap.h"

class STable {
  private:
    Bitmap* bm;
    Sem* semTab[MAX_SEMAPHORE];
    // quan ly slot trong
    // quan ly toi da 10 doi tuong Sem
  public:
    // khoi tao size đoi tuong Sem đe quan ly 10 Semaphore. Gan gia tri ban đau la null
    // nho khoi tao bm đe su dung
    STable();

    ~STable();

    int Create(char* name, int init);

    int Wait(char* name);
    
    int Signal(char* name);
    
    int FindFreeSlot();
};


#endif