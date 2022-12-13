/***************************************************************************************
** FILE NAME      : server.cpp
** DESCRIPTION    : The server program is created to accept the client and receive data 
                    from client and storing in a structure
** GROUP          : group7
** DATE           : 12/12/2022
***************************************************************************************/

#include <server.h>
#include <shared_mem.h>


int AddToDataBase(SharedMemory &,data);
int DeleteFromDatabase(SharedMemory &,data);
int FindData(SharedMemory &,data,int &position);
int EditDataBase(SharedMemory &,data,int position);

/***************************************************************************************
** FUNCTION NAME    : CreateSocket
** DESCRIPTION      : create the socket communication with domain IPv4 Internet Protocol
                      and type TCP
** RETURN TYPE      : int
***************************************************************************************/
int SocketServer::CreateSocket ()
{
    this->socket_fd=socket (AF_INET, SOCK_STREAM, 0);
    if (this->socket_fd < 0)
        return FAILED_;
    return SUCCESS;
}

/***************************************************************************************
** FUNCTION NAME    : Bind
** DESCRIPTION      : populate the sockaddr structute with proper domain,type and IP
                      for accepting connetions
** RETURN TYPE      : int
***************************************************************************************/
int SocketServer::Bind ()
{
    this->serv_addr.sin_family = AF_INET;             
    this->serv_addr.sin_addr.s_addr = INADDR_ANY;     
    this->serv_addr.sin_port = htons (this->port); 

    return bind(this->socket_fd, (struct sockaddr *) &this->serv_addr, sizeof (this->serv_addr));
}

/***************************************************************************************
** FUNCTION NAME    : Listen
** DESCRIPTION      : listen system call wait for the client to make a connection
** RETURN TYPE      : int
***************************************************************************************/
int SocketServer::Listen ()
{
    if (listen (this->socket_fd, MAX_CONNECTIONS) < 0)
        return FAILED_;
    return SUCCESS;
}

/***************************************************************************************
** FUNCTION NAME    : Accept
** DESCRIPTION      : accept is a blocking call and wait for the connection
** RETURN TYPE      : int
***************************************************************************************/
void SocketServer::Accept ()
{
    int pid;
    cout<<"[INFO]: Server is Running"<<endl;
    while (1)
    {
        this->data_socket_fd = accept (this->socket_fd, (struct sockaddr *) NULL, NULL);

        if (this->data_socket_fd > 0)
            cout << "[INFO]: Connection accepted with ID: " << this->data_socket_fd << endl;
        else
            cout << "[ERROR]: Connection Not accepted\n" << endl;

        pid = fork ();

	//child process handles the current connection while parent process waits for the new connection 
        if (pid == 0)
        {
            //sharedmemory class handles the API'S of our database
            SharedMemory DataBase;

	    //recv function reads from data socket file descriptor and populates it to the structure
            recv(this->data_socket_fd,(data *)&(this->Data),sizeof(data), 0);

            switch (this->Data.choice)
            {

                case ADD:
                    {
			int position{0};
			if(FindData(DataBase,this->Data,position) == FAILED_)
			{
                        	if(AddToDataBase(DataBase,this->Data) == FAILED_)
                            		send(this->data_socket_fd,(void*)FAILURE_MSG,MSG_SIZE,0);
                        	else
                            		send(this->data_socket_fd,(void*)SUCCESS_MSG,MSG_SIZE,0);
			}
			else{
				send(this->data_socket_fd, (void*)FAILURE_MSG,MSG_SIZE,0);
			}
                        break;
                    }
                case EDIT:
                    {
                        int position{0};

                        //find the data is available or not
                        if(FindData(DataBase,this->Data,position) == FAILED_)
                        {
                            send(this->data_socket_fd,(void*)FAILURE_MSG,MSG_SIZE,0);
                        }
                        else
                        {
			    //if data found again receive new data to edit
                            send(this->data_socket_fd,(void*)SUCCESS_MSG,MSG_SIZE,0);
                            recv(this->data_socket_fd,(data *)&(this->Data),sizeof(data),0);
			    //send acknowlegement to client whether edit success or failure
                            if(EditDataBase(DataBase,this->Data,position) == FAILED_)
                            {

                                send(this->data_socket_fd,(void*)FAILURE_MSG,MSG_SIZE,0);
                            }
                            else
                               	send(this->data_socket_fd,(void*)SUCCESS_MSG,MSG_SIZE,0);
			}
                        
                        break;
                    }
                case DELETE:
                    {
                        if(DeleteFromDatabase(DataBase,this->Data) == FAILED_)
                            send(this->data_socket_fd,(void*)FAILURE_MSG,MSG_SIZE,0);
                        else
                            send(this->data_socket_fd,(void*)SUCCESS_MSG,MSG_SIZE,0);
                        break;
                    }
                default:
                    cout<<"[ERROR]: Invalid choice"<<endl;

            }
            close (this->data_socket_fd);
            //exit from the child.
            exit (0);
        }
    }
}
int AddToDataBase(SharedMemory &DataBase,data Data)
{
    if (DataBase.CreateSharedMemory () == FAILED_)
        cout << "[INFO]: Shared memory already exists" << endl;

    if (DataBase.AttachToMemory () == FAILED_)
    {
        cout << "[ERROR]: Attaching to shared memory failed" <<endl;
        return FAILED_;
    }

    if (DataBase.AddData (Data) == SUCCESS)
    {
        cout << "[INFO]: Employee list after addition" << endl;
        DataBase.CheckData();
    }
    else
        return FAILED_;

    if (DataBase.DetachFromMemory () == FAILED_)
    {
        cout << "[INFO]: Detaching from the shared memory Failed" <<endl;
        return FAILED_;
    }

    return SUCCESS;
}
int FindData(SharedMemory &DataBase,data Data,int &position)
{

    if (DataBase.CreateSharedMemory() == FAILED_)
        cout << "[INFO]: Shared memory already exists" << endl;
    if (DataBase.AttachToMemory() == FAILED_)
    {
        cout << "[ERROR]: Attaching to shared memory failed" <<endl;
        return FAILED_;
    }

    if (DataBase.FindData(Data,position) == SUCCESS)
        return SUCCESS;
    if (DataBase.DetachFromMemory() == FAILED_)
        cout <<"[INFO]: Detaching from the shared memory Failed" <<endl;
    return FAILED_;


}
int EditDataBase(SharedMemory &DataBase,data Data,int position)
{

    if (DataBase.CreateSharedMemory() == FAILED_)
        cout << "[INFO]: Shared memory already exists" << endl;
    if (DataBase.AttachToMemory() == FAILED_)
    {
        cout << "[ERROR]: Attaching to shared memory failed" <<endl;
        return FAILED_;
    }

    if (DataBase.EditData(Data,position) == SUCCESS)
    {
        cout << "[INFO]: Employee list after Edit" << endl;
        DataBase.CheckData();
    }
    else
        return FAILED_;
    if (DataBase.DetachFromMemory() == FAILED_)
        cout <<"[INFO]: Detaching from the shared memory Failed" <<endl;
    return SUCCESS;
}
int DeleteFromDatabase(SharedMemory &DataBase,data Data)
{

    if (DataBase.CreateSharedMemory () == FAILED_)
        cout << "[INFO]: Shared memory already exists" << endl;
    if (DataBase.AttachToMemory () == FAILED_)
    {
        cout << "[ERROR]: Attaching to shared memory failed" <<endl;
        return FAILED_;
    }
    if (DataBase.DeleteData (Data) == SUCCESS)
    {
        cout << "[INFO]: Employee list after deleteion" << endl;
        DataBase.CheckData ();
    }
    else
    {
        if (DataBase.DetachFromMemory () == FAILED_)
            cout <<"[INFO]: Detaching from the shared memory Failed" <<endl;
        return FAILED_;
    }
    if (DataBase.DetachFromMemory () == FAILED_)
        cout <<"[INFO]: Detaching from the shared memory Failed" <<endl;
    return SUCCESS;
}
