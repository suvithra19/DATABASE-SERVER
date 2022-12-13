#include "client.h"

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
