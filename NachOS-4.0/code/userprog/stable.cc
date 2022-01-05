#include "stable.h"

STable::STable() {
	bm = new Bitmap(MAX_SEMAPHORE);

	for (int i = 0; i < MAX_SEMAPHORE; i++) {
		semTab[i] = NULL;
	}
	bm->Mark(0);
}

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
	for(int i=0; i<MAX_SEMAPHORE; i++) {
		if(bm->Test(i)) {
			if(strcmp(name, semTab[i]->GetName()) == 0) {
				return -1;
			}
		}
	}
	// Tim slot tren bang semTab trong
	int id = this->FindFreeSlot();
	
	// Neu k tim thay thi tra ve -1
	if(id < 0) {
		return -1;
	}

	// Neu tim thay slot trong thi nap Semaphore vao semTab[id]
	this->semTab[id] = new Sem(name, init);
	return 0;
}

int STable::Wait(char* name) {
	for(int i =0; i < MAX_SEMAPHORE; i++) {
		// Kiem tra o thu i da duoc nap semaphore chua
		if(bm->Test(i)) {
			// Neu co thi tien hanh so sanh name voi name cua semaphore trong semTab
			if(strcmp(name, semTab[i]->GetName()) == 0) {
				// Neu ton tai thi cho semaphore down(); 
				semTab[i]->wait();
				return 0;
			}
		}
	}
	printf("Khong ton tai semaphore");
	return -1;
}

int STable::Signal(char* name) {
	for(int i =0; i < MAX_SEMAPHORE; i++) {
		// Kiem tra o thu i da duoc nap semaphore chua
		if(bm->Test(i)) {
			// Neu co thi tien hanh so sanh name voi name cua semaphore trong semTab
			if(strcmp(name, semTab[i]->GetName()) == 0) {
				// Neu ton tai thi cho semaphore up(); 
				semTab[i]->signal();
				return 0;
			}
		}
	}
	printf("Khong ton tai semaphore");
	return -1;
}

int STable::FindFreeSlot() {
	return this->bm->FindAndSet();
}