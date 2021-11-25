#ifndef Network_H
#define Network_H

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT   "3333"

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <string>
#include <GameManager.h>

typedef enum
{
	Server_instance,
	Client_instance
}Network_entity;


class Network
{
	public:
	bool is_connected;
	bool main_player;
	Network_entity role;
	PlayMode mode;
	std::string IP;
	//windows atributes
	WSADATA wsaData;
	struct addrinfo *result = NULL;
	struct addrinfo *ptr    = NULL;
	struct addrinfo hints;

	int recvbuflen       = DEFAULT_BUFLEN;
	char recvbuf[DEFAULT_BUFLEN];
                     
	int iResult;

	Network(Network_entity net,std::string ip_str);
	virtual ~Network() {};

	//Custom Functions
	virtual void Establish_Communication() = 0;
	virtual void Send(std::string msg)  = 0;
	virtual void Recieve() = 0;
	virtual void Load_Game() = 0;
	virtual void Play_Game() = 0;
	virtual void Set_Main_Player() = 0;
};

#endif
