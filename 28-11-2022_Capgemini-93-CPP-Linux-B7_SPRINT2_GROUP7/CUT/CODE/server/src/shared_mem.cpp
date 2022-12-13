/**************************************************************************************
** FILE NAME     : shared_mem.cpp
** DESCRIPTION   : API's for database resides here 
** GROUP         : group7
** DATE          : 12/12/2022
**************************************************************************************/

#include <shared_mem.h>

int SharedMemory::CreateSharedMemory()
{
   
    this->shmid=shmget (0x1234,sizeof(struct user_data), 0644|IPC_CREAT);
    if(this->shmid == -1)
        return FAILED_;
    else
        return SUCCESS;
}
int SharedMemory::AttachToMemory()
{
 
    this->data=(UserData*)shmat (this->shmid, 0, 0);
    if(this->data == NULL)
        return FAILED_;
    else
        return SUCCESS;
}
int SharedMemory::AddData(struct Data data)
{

    strcpy(this->data->name[this->data->synnum],data.name);
    this->data->number[this->data->synnum]=data.number;
    this->data->ID[this->data->synnum]=data.ID;
    this->data->synnum++;
    return SUCCESS;
}
int SharedMemory::FindData(struct Data data,int &position)
{
    //finding data in database.
    for(int i=0;i<MAX_MEM_SIZE;i++)
    {
        if(this->data->ID[i]==data.ID)
        {
            position=i;
            return SUCCESS;
        }
    }
    return FAILED_;
}
int SharedMemory::EditData(struct Data data,int position)
{
 
    strcpy(this->data->name[position],data.name);
    this->data->ID[position]=data.ID;
    this->data->number[position]=data.number;
    return SUCCESS;
}

int SharedMemory::DeleteData(struct Data data)
{
  
    for(int i=0;i<MAX_MEM_SIZE;i++)
    {
        if(this->data->ID[i]==data.ID)
        {
            for(int j=i;j<MAX_MEM_SIZE-1;j++)
            {
                strcpy(this->data->name[j],this->data->name[j+1]);
                this->data->number[j]=this->data->number[j+1];
                this->data->ID[j]=this->data->ID[j+1];
            }
            this->data->synnum--;
            return SUCCESS;
        }
    }
    return FAILED_;
}

int SharedMemory::DetachFromMemory()
{
    //detach from the shared memory.
    shmdt (this->data);
    return SUCCESS;
}
void SharedMemory::CheckData()
{
 
    cout<<endl;
    cout<<"------------------------------"<<endl;
    cout<<"|      EMPLOYEE DATABASE      |"<<endl;
    cout<<"------------------------------"<<endl;

    for(int i=0;i<MAX_MEM_SIZE;i++)
    {
        if(this->data->number[i] == 0)
            break;
        cout<<std::left<<std::setw(20);
        cout<<this->data->name[i];
        cout<<std::left<<std::setw(10);
        cout<<this->data->ID[i];
        cout<<std::left<<std::setw(10);
        cout<<this->data->number[i]<<endl;
    }
    cout<<"------------------------------"<<endl;
    cout<<endl;
}
