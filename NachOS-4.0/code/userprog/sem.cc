#include "sem.h"

// Khoi tao doi tuong Sem. Gan gia tri ban dau la null
// nho khoi tao bm su dung
Sem::Sem(char *na, int i)
{
    strcpy(this->name, na);
    sem = new Semaphore(na, i);
}

// Huy cac doi tuong da tao
Sem::~Sem()
{
    delete sem;
}

// Thuc hien thao tac cho
void Sem::wait()
{
    sem->P();
}

// Thuc hien thao tac giai phong Semaphore
void Sem::signal()
{
    sem->V();
}

// Tra ve ten cua Semaphore
char* Sem::GetName()
{
    printf("Get Name sem \n");
    return name;
}