#include "shared_mem.h"

int SharedMemory::CreateSharedMemory()
{
    //shmget creates shared memory of specified key if it exits it returns +ve value.
    //more info on man shmget.
    this->shmid=shmget (0x1234,sizeof(struct user_data), 0644|IPC_CREAT);
    if(this->shmid == -1)
        return FAILED_;
    else
        return SUCCESS;
}
int SharedMemory::AttachToMemory()
{
    //shmat attaches the structure we specify to the shared memory of specified key.
    this->empdata=(UserData*)shmat (this->shmid, 0, 0);
    if(this->empdata == NULL)
        return FAILED_;
    else
        return SUCCESS;
}
int SharedMemory::AddData(struct eData empdata)
{
    //populating data into the structure
    strcpy(this->empdata->name[this->empdata->synnum],empdata.name);
    this->empdata->number[this->empdata->synnum]=empdata.number;
    this->empdata->ID[this->empdata->synnum]=empdata.ID;
    this->empdata->synnum++;
    return SUCCESS;
}
int SharedMemory::FindData(struct eData empdata,int &position)
{
    //finding data in database.
    for(int i=0;i<MAX_MEM_SIZE;i++)
    {
        if(!(strcmp(this->empdata->name[i],empdata.name)))
        {
            position=i;
            return SUCCESS;
        }
    }
    return FAILED_;
}
int SharedMemory::EditData(struct eData empdata,int position)
{
    //change the old name to new name in position found by find.
    strcpy(this->empdata->name[position],empdata.name);
    this->empdata->ID[position]=empdata.ID;
    this->empdata->number[position]=empdata.number;
    return SUCCESS;
}

int SharedMemory::DeleteData(struct eData empdata)
{
    //find the data to delete and also adjust the space so that there are no leakage.
    //this is an O(n^2) solution need to improve it future to atleast O(n).
    for(int i=0;i<MAX_MEM_SIZE;i++)
    {
        if(!(strcmp(this->empdata->name[i],empdata.name)))
        {
            for(int j=i;j<MAX_MEM_SIZE-1;j++)
            {
                strcpy(this->empdata->name[j],this->empdata->name[j+1]);
                this->empdata->number[j]=this->empdata->number[j+1];
                this->empdata->ID[j]=this->empdata->ID[j+1];
            }
            this->empdata->synnum--;
            return SUCCESS;
        }
    }
    return FAILED_;
}

int SharedMemory::DetachFromMemory()
{
    //detach from the shared memory.
    shmdt (this->empdata);
    return SUCCESS;
}
void SharedMemory::CheckData()
{
    //Displaying the Employee list after processing from client.
    cout<<endl;
    cout<<"------------------------------"<<endl;
    cout<<"|      EMPLOYEE DATABASE      |"<<endl;
    cout<<"------------------------------"<<endl;

    for(int i=0;i<MAX_MEM_SIZE;i++)
    {
        if(this->empdata->number[i] == 0)
            break;
            
        
        cout<<std::left<<std::setw(20);
        cout<<this->empdata->name[i];
        cout<<std::left<<std::setw(10);
        cout<<this->empdata->ID[i];
        cout<<std::left<<std::setw(10);
        cout<<this->empdata->number[i]<<endl;
    }
    cout<<"------------------------------"<<endl;
    cout<<endl;
}
