#ifndef Client_H
#define Client_H

#include "Network.h"


class Client : Network
{
	public:
	SOCKET ConnectSocket = INVALID_SOCKET;
	Client(std::string address);
	void Establish_Communication();
 	void Load_Game();
	void Play_Game();
	void Set_Main_Player();
	
};

#endif
