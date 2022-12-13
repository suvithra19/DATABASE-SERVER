#include "client.h"

int SocketClient::CreateSocket()
{
    this->socket_fd=socket(AF_INET, SOCK_STREAM, 0);
    if(this->socket_fd < 0)
        return FAILED_;
    return SUCCESS;
}
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
int SocketClient::SendAndClose()
{
    switch(this->eData.choice)
    {
        case ADD:

            if(send(this->socket_fd, (empdata*)&(this->eData),sizeof(empdata),0))
                cout<<"[INFO]: DATA sent to server successsfully"<<endl;
            else
                return FAILED_;

            recv(this->socket_fd,(void*)this->ServerMsg,SERVER_MSG_SIZE,0);
            if(!strcmp(this->ServerMsg,FAILURE_MSG))
                return FAILED_;
            else
                cout<<"[SERVER INFO]: ADD SUCCESS"<<endl;
            break;

        case EDIT:

            if(send(this->socket_fd, (empdata*)&(this->eData),sizeof(empdata),0))
                cout<<"[INFO]: DATA sent to server successsfully"<<endl;
            else
                return FAILED_;
            if(HandleEdit() == FAILED_)
            {
                cout<<"[SERVER ERROR]: No such name exists"<<endl;
                return FAILED_;
            }
            else
                cout<<"[SERVER INFO]: EDIT SUCCESS"<<endl;
            break;
        //delete user data in server.
        case DELETE:

            //check if data exist in server to delete else return error.
            if(send(this->socket_fd, (empdata*)&(this->eData),sizeof(empdata),0))
                cout<<"[INFO]: DATA sent to server successsfully"<<endl;
            else
                return FAILED_;

            recv(this->socket_fd,(void*)this->ServerMsg,SERVER_MSG_SIZE,0);

            if(!strcmp(this->ServerMsg,FAILURE_MSG))
                cout<<"[SERVER ERROR]: No such name exists"<<endl;
            else
                cout<<"[SERVER INFO]: DELETE SUCCESS"<<endl;
            break;

    }
    //close the socket and return.
    close(socket_fd);
    return SUCCESS;
}
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
            cin>>this->eData.name;
	    cout<<"Id>>";
	    cin>>this->eData.Id;
            cout<<"NUMBER>>";
            cin>>this->eData.number;
            this->eData.choice=choice;
            break;
        case 2:
            cout<<"Enter the employee name to edit:"<<endl;
            cout<<"NAME>>";
            cin>>this->eData.name;
            this->eData.choice=choice;
            break;
        case 3:
            cout<<"Enter the employee name to delete:"<<endl;
            cout<<"NAME>>";
            cin>>this->eData.name;
            this->eData.choice=choice;
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
        cout<<"[ERROR]: coudn't find the name to edit"<<endl;
        return FAILED_;
    }
    else
    {
        cout<<"Enter the new name:";
        cin>>this->eData.name;
	cout<<"Enter new Id:";
	cin>>this->eData.Id;
	cout<<"Enter new number:";
	cin>>this->eData.number;
        if(send(this->socket_fd,(empdata*)&this->eData,sizeof(eData),0))
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
