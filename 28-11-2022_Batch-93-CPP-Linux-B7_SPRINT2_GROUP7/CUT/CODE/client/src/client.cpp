/***************************************************************************************
** FILE NAME      : client.cpp
** DESCRIPTION    : The client program is created to get server and send the Employee 
                    data to the server
** GROUP          : group7
** DATE           : 12/12/2022
***************************************************************************************/
#include <client.h>

/***************************************************************************************
** FUNCTION NAME    : CreateSocket
** DESCRIPTION      : create the socket for communication with domain IPv4 Internet
                      Protocol and type TCP
** RETURN TYPE      : int 
***************************************************************************************/
int SocketClient::CreateSocket()
{
    this->socket_fd=socket(AF_INET, SOCK_STREAM, 0);
    if(this->socket_fd < 0)
        return FAILED_;
    return SUCCESS;
}

/***************************************************************************************
** FUNCTION NAME    : Bind
** DESCRIPTION      : populate the socaddr structure with proper domain,type and IP for 
                      connecting to server
** RETURN TYPE      : int 
***************************************************************************************/
int SocketClient::Bind()
{
    char* IP= new char[sizeof(this->address.c_str())];
    strcpy(IP,this->address.c_str());
    this->ServerInfo.sin_family = AF_INET;
    this->ServerInfo.sin_addr.s_addr = inet_addr(IP);
    this->ServerInfo.sin_port = htons(this->port);
    delete[] IP;
    return SUCCESS;
}

/***************************************************************************************
** FUNCTION NAME    : Connect
** DESCRIPTION      : connect is a blocking call, returns zero if connection is 
                      successful else returns -1  
** RETURN TYPE      : int 
***************************************************************************************/
int SocketClient::Connect()
{
    if(connect(socket_fd, (struct sockaddr *)&this->ServerInfo, sizeof(this->ServerInfo)) != 0)
    {
        cout << "[ERROR]: Error connecting to server"<< endl;
        return FAILED_;
    }
    cout<<"[INFO]: Connection successfull"<< endl;

    return SUCCESS;
}

/***************************************************************************************
** FUNCTION NAME    : SendAndClose
** DESCRIPTION      : send the data to the server according to the user needs and closes 
                      the socket connection
** RETURN TYPE      : int 
***************************************************************************************/
int SocketClient::SendAndClose()
{
    switch(this->Data.choice)
    {
	//add user data to server
        case ADD:

            if(send(this->socket_fd, (data*)&(this->Data),sizeof(data),0))
                cout<<"[INFO]: DATA sent to server successsfully"<<endl;
            else
                return FAILED_;

            recv(this->socket_fd,(void*)this->ServerMsg,SERVER_MSG_SIZE,0);
            if(!strcmp(this->ServerMsg,FAILURE_MSG)){
		cout<<"[SERVER INFO]: Employee ID already exist"<<endl;
                return FAILED_;
	    }
            else
                cout<<"[SERVER INFO]: ADD SUCCESS"<<endl;
            break;

	//edit user data in server
        case EDIT:

            if(send(this->socket_fd, (data*)&(this->Data),sizeof(data),0))
                cout<<"[INFO]: DATA sent to server successsfully"<<endl;
            else
                return FAILED_;
            if(HandleEdit() == FAILED_)
            {
                cout<<"[SERVER ERROR]: No such Employee ID exist "<<endl;
                return FAILED_;
            }
            else
                cout<<"[SERVER INFO]: EDIT SUCCESS"<<endl;
            break;

	//delete user data in server
        case DELETE:

            if(send(this->socket_fd, (data*)&(this->Data),sizeof(data),0))
                cout<<"[INFO]: DATA sent to server successsfully"<<endl;
            else
                return FAILED_;

            recv(this->socket_fd,(void*)this->ServerMsg,SERVER_MSG_SIZE,0);

            if(!strcmp(this->ServerMsg,FAILURE_MSG))
                cout<<"[SERVER ERROR]: No such Employee ID exists"<<endl;
            else
                cout<<"[SERVER INFO]: DELETE SUCCESS"<<endl;
            break;

    }
    //close the socket and return
    close(socket_fd);
    return SUCCESS;
}


/***************************************************************************************
** FUNCTION NAME    : DisplayMenu
** DESCRIPTION      : display the user menu to choose the operation needs and return
                      the choice
** RETURN TYPE      : int 
***************************************************************************************/
int SocketClient::DisplayMenu()
{
    int choice{0};
    cout<<" ---------------------"<<endl;
    cout<<"|Welcome to the server|"<<endl;
    cout<<" ---------------------"<<endl;
    cout<<"Choose any of the following options"<<endl<<endl;
    cout<<"1:ADD DATA"<<endl;
    cout<<"2:EDIT DATA"<<endl;
    cout<<"3:DELETE DATA"<<endl<<endl;
    cout<<"Enter the option:";
    cin>>choice;
    switch(choice)
    {
        case 1:
            cout<<"Enter the employee details to add:"<<endl;
            cout<<"NAME>>";
            cin>>this->Data.name;
	    cout<<"Id>>";
	    cin>>this->Data.Id;
            cout<<"NUMBER>>";
            cin>>this->Data.number;
            this->Data.choice=choice;
            break;
        case 2:
            cout<<"Enter the employee ID to edit:"<<endl;
            cout<<"ID>>";
            cin>>this->Data.Id;
            this->Data.choice=choice;
            break;
        case 3:
            cout<<"Enter the employee ID to delete:"<<endl;
            cout<<"ID>>";
            cin>>this->Data.Id;
            this->Data.choice=choice;
            break;
        default:
            cout<<"wrong option"<<endl;
    }
    return choice;
}
int SocketClient::HandleEdit()
{
    recv(this->socket_fd,(void*)this->ServerMsg,SERVER_MSG_SIZE,0);
    if(!strcmp(this->ServerMsg,FAILURE_MSG))
    {
        cout<<"[ERROR]: coudn't find the Employee ID to edit"<<endl;
        return FAILED_;
    }
    else
    {
        cout<<"Enter the new name:";
        cin>>this->Data.name;
	cout<<"Enter new Id:";
	cin>>this->Data.Id;
	cout<<"Enter new number:";
	cin>>this->Data.number;
        if(send(this->socket_fd,(data*)&this->Data,sizeof(Data),0))
            cout<<"[INFO]: DATA sent to server successsfully"<<endl;
        else
            return FAILED_;
        recv(this->socket_fd,(void*)this->ServerMsg,SERVER_MSG_SIZE,0);
        if(!strcmp(this->ServerMsg,FAILURE_MSG))
        {
            return FAILED_;
        }
    }
    return SUCCESS;
}
