#ifndef Client_H
#define Client_H
#define DEFAULT_BUFLEN 512

#include "Network.h"

class Client : Network
{
	public:
	SOCKET ConnectSocket = INVALID_SOCKET;
	Client(std::string address);
	~Client();
	void Establish_Communication();
 	void Load_Game();
	void Play_Game();
	void Set_Main_Player();
	void Send();
	void Recieve();
};

#endif
