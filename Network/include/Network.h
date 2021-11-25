#ifndef Network_H
#define Network_H

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <string>

class Network
{
	public:
	bool is_connected;
	bool main_player;
	std::string IP;
	//windows atributes
	WSADATA wsaData;
	struct addrinfo *result = NULL,
                    *ptr    = NULL,
                     hints;
                     
	int iResult;
	
	//Custom Functions
	virtual void Establish_Communication() = 0;
	virtual void Load_Game() = 0;
	virtual void Play_Game() = 0;
	virtual void Set_Main_Player() = 0;
	
	virtual ~Network() {};
};

#endif
