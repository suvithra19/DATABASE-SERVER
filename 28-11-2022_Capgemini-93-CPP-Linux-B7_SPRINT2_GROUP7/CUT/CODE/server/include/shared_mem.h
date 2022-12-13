/***************************************************************************************
** FILE NAME      : shared_mem.h
** DESCRIPTION    : class and structure declarations and library files are here
** GROUP          : group7
** DATE           : 12/12/2022
****************************************************************************************/

#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <sys/shm.h>
#include <cstring>
#include "server.h"
#define MAX_MEM_SIZE 100
#define FAILED_ -1
#define SUCCESS 0
using namespace std;

typedef struct user_data{

    int ID[MAX_MEM_SIZE];
    long number[MAX_MEM_SIZE];
    char name[MAX_MEM_SIZE][100];
    int  synnum;

}UserData;

class SharedMemory
{
    public:
        SharedMemory():data(nullptr){};
        int CreateSharedMemory();
        int AttachToMemory();
        int AddData(struct Data data);
        int FindData(struct Data data,int &position);
        int EditData(struct Data data,int position);
        int DeleteData(struct Data data);
        int DetachFromMemory();
        void CheckData();
    private:
        int shmid;
        UserData *data;

};

#endif
