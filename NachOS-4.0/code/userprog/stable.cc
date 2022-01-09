#include "stable.h"

// Tim ID cua Semaphore "name". Neu khong co tra ve -1
int STable::FindByName(char* name){
    for(int id=0; id<MAX_SEMAPHORE; id++) {
		// Kiem tra o thu i da duoc nap semaphore chua
		if(bm->Test(id) && semTab[id] != NULL ) {
			// Neu co thi tien hanh so sanh name voi name cua semaphore trong semTab
			if(strcmp(name, semTab[id]->GetName()) == 0){
				return id;
			}
		}
	}
    // Neu khong co thi tra ve -1
    return -1;
}

// Khoi tao size đoi tuong Sem đe quan ly 10 Semaphore. Gan gia tri ban đau la null
STable::STable() {
	bm = new Bitmap(MAX_SEMAPHORE);

	for (int i = 0; i < MAX_SEMAPHORE; i++) {
		semTab[i] = NULL;
	}
	bm->Mark(0);
}

// Huy cac doi tuong da tao
STable::~STable() {
	if (bm != NULL)
		delete bm;
	for (int i = 0; i < MAX_SEMAPHORE; i++)
		if (semTab[i] != NULL)
			delete semTab[i];
}

// Kiem tra Semaphore "name" chua ton tai thi tao Semaphore moi
int STable::Create(char* name, int init) {
	// Check da ton tai semaphore nay chua?
    if (FindByName(name) >= 0) {
        printf("Da ton tai semaphore");
        return -1;
    }

	// Tim 1 slot tren bang semTab
    // Neu tim thay thi danh dau da su dung
	// Neu khong tim thay thi tra ve -1
	int id = FindFreeSlot();
	
    // Neu khong tim thay
	if(id < 0) {
		return -1;
	}

	// Neu tim thay slot trong thi nap Semaphore vao semTab[id]
	semTab[id] = new Sem(name, init);
	return 0;
}

// Kiem tra Semaphore "name" ton tai thi goi wait()
int STable::Wait(char* name) {
    int id = FindByName(name);
    if (id < 0) {
        printf("Khong ton tai semaphore");
        return -1;
    }
    semTab[id]->wait();
    return 0;
}

// Kiem tra Semaphore "name" ton tai thi goi signal()
int STable::Signal(char* name) {
    int id = FindByName(name);
    if (id < 0) {
        printf("Khong ton tai semaphore");
        return -1;
    }
    semTab[id]->signal();
    return 0;
}

// Tim slot trong 
int STable::FindFreeSlot() {
	return bm->FindAndSet();
}