/****************************************************************************************
** FILE NAME     : client.h
** DESCRIPTION   : class and structure declaration and library files are here
** GROUP         : group7
** DATE          : 12/12/2022
****************************************************************************************/

#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#define ADD 1
#define EDIT 2
#define DELETE 3
#define SUCCESS_MSG "ALL OK"
#define FAILURE_MSG "NOT OK"
#define MAX_NAME_LENGTH 100
#define SERVER_MSG_SIZE 6
#define FAILED_ -1
#define SUCCESS 0

using namespace std;

typedef struct Data
{
    int  choice;
    int Id;
    long number;
    char name[MAX_NAME_LENGTH];
}data;

class SocketClient
{
    public:
        SocketClient(std::string ip="127.0.0.1",int port=5000):port(port),address(ip){}
        ~SocketClient(){};
        void getinfo(){cout<<this->address<<" "<<this->port<<endl;}
        int  CreateSocket();
        int  Bind();
        int  Connect();
        int  DisplayMenu();
        int  SendAndClose();
        int  HandleEdit();

    private:
        int     port;
        int     socket_fd;
        char    ServerMsg[SERVER_MSG_SIZE];
        data    Data;
        struct  sockaddr_in ServerInfo;
        string  address;

};

#endif 
