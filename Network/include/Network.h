#ifndef Network_H
#define Network_H

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT   "3333"

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
	struct addrinfo *result = NULL;
	struct addrinfo *ptr    = NULL;
	struct addrinfo hints;

	int recvbuflen       = DEFAULT_BUFLEN;
	char recvbuf[DEFAULT_BUFLEN];
                     
	int iResult;
	
	//Custom Functions
	virtual void Establish_Communication() = 0;
	virtual void Send()    = 0;
	virtual void Recieve() = 0;
	virtual void Load_Game() = 0;
	virtual void Play_Game() = 0;
	virtual void Set_Main_Player() = 0;
	
	virtual ~Network() {};
};

#endif
