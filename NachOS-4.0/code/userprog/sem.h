#ifndef SEM_H
#define SEM_H

#include "synch.h"

class Semaphore;

class Sem
{
    private:
        char name[50];
        Semaphore *sem; // Tao semaphore de quan ly

    public:
        // Khoi tao doi tuong Sem. Gan gia tri ban dau la null
        // nho khoi tao bm su dung
        Sem(char *na, int i);

        // Huy cac doi tuong da tao
        ~Sem();

        // Thuc hien thao tac cho
        void wait();

        // Thuc hien thao tac giai phong Semaphore
        void signal();
        
        // Tra ve ten cua Semaphore
        char *GetName();
};


#endif