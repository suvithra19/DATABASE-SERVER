/***************************************************************************************
** FILE NAME      : server_main.cpp 
** DESCRIPTION    : driver function for server and datas are storing in the server
** GROUP          : group7
** DATE           : 12/12/2022
****************************************************************************************/

#include <server.h>


int main()
{

    SocketServer soc;                    

    if(soc.CreateSocket() == FAILED_)    
    {
        perror("[ERROR]");
        exit(EXIT_FAILURE);
    }

    if(soc.Bind() == FAILED_)          
    {                               
        perror("[ERROR]");
        exit(EXIT_FAILURE);
    }

    if(soc.Listen() == FAILED_)      
    {                              
        perror("[ERROR]");
        exit(EXIT_FAILURE);
    }

    soc.Accept();                      

 
    return 0;
}  
