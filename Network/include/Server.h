#ifndef SERVER_H
#define SERVER_H

#include "Network.h"

class Server : Network
{
    public:
    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;
    int iSendResult;
    Server();
    ~Server();
    void Establish_Communication();
 	void Load_Game();
	void Play_Game();
	void Set_Main_Player();
	void Send();
	void Recieve();
}; 


#endif