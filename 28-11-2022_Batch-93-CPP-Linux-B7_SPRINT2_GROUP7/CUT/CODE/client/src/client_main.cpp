/***************************************************************************************
** FILE NAME      : client_main.cpp
** DESCRIPTION    : driver function for the client to connect to the server and data
                    transfer 
** GROUP          : group7
** DATE           : 12/12/2022
***************************************************************************************/
#include <client.h>

int main()
{
    SocketClient soc;                     

    if(soc.CreateSocket() == FAILED_)    
    {
        perror ("[ERROR]: ");
        exit(EXIT_FAILURE);
    }

    if(soc.Bind() == FAILED_)           
    {                                  
        perror ("[ERROR]: ");
        exit(EXIT_FAILURE);
    }

    if(soc.Connect() == FAILED_)      
    { 
        perror ("[ERROR]: ");
        exit(EXIT_FAILURE);
    }

    soc.DisplayMenu();               

    if(soc.SendAndClose() == FAILED_)
    { 
        perror ("[ERROR]: ");
        exit(EXIT_FAILURE);
    }

    return 0;
}
