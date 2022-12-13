#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <sys/shm.h>
#include <cstring>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "server.h"
#define MAX_MEM_SIZE 100
#define SHM_NAME  "/shm_example1"
#define FAILED_ -1
#define SUCCESS 0
using namespace std;

typedef struct user_data{

    //only static memory allocations are allowed in shared memory
    //assigning pointers to shared memory is a mess.
    int ID[MAX_MEM_SIZE];
    long number[MAX_MEM_SIZE];
    char name[MAX_MEM_SIZE][100];
    int  synnum;

}UserData;

class SharedMemory
{
    public:
        SharedMemory():empdata(nullptr){};
        int CreateSharedMemory();
        int AttachToMemory();
        int AddData(struct eData empdata);
        int FindData(struct eData empdata,int &position);
        int EditData(struct eData empdata,int position);
        int DeleteData(struct eData empdata);
        int DetachFromMemory();
        void CheckData();
    private:
        int shmid;
        UserData *empdata;

};

#endif //SHAREDMEMORY_H
