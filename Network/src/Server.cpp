#include <stdio.h>
#include <iostream>
#include "Server.h"

#pragma comment (lib, "Ws2_32.lib")

Server::Server():Network(Server_instance,"")
{
    std::cout<<"server build"<<std::endl;
    this->mode     = Play;
    this->cuadrant = Left_Cuadrant;
}

void Server::Establish_Communication()
{
    // Create a SOCKET for connecting to server
    printf("creating socket\r\n");
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    printf("listen socket created; %d\r\n",ListenSocket);

    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return;
    }
    // BINDING
    printf("bind\r\n");
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }
    printf("Binding\n");
    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }
    int           nRet;
    unsigned long ul = 1;
    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    nRet = ioctlsocket(ClientSocket, FIONBIO, (unsigned long *) &ul);
    
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }
    printf("Client accept\n");
}
void Server::Load_Game()
{

}
void Server::Play_Game()
{

}
void Server::Set_Main_Player()
{

}
void Server::Send(std::string msg)
{
    // Echo the buffer back to the sender
    iSendResult = send(ClientSocket, msg.c_str(), msg.size(), 0 );
    if (iSendResult == SOCKET_ERROR) {
        //printf("send failed with error: %d\n", WSAGetLastError());
        //closesocket(ClientSocket);
        //WSACleanup();
        return;
    }
    printf("Bytes sent: %d\n", iSendResult);
}

inline void Server::Recieve()
{
    memset(recvbuf,0,recvbuflen);
    iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
    if(iResult == WSAEWOULDBLOCK || iResult == -1){return;}
    if (iResult > 0) {
        printf("Bytes received: %d\n", iResult);
        printf("msg: %s\n", recvbuf);
    }
    else if (iResult == 0)
    {
        printf("Connection closing...\n");
    }
    else if(iResult == WSAEWOULDBLOCK){
		printf("holdon\r\n");
	}
    else {
        printf("recv failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
    }
}
Server:: ~Server()
{
    // cleanup
    closesocket(ClientSocket);
    WSACleanup();
}
