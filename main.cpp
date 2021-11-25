#include <Client.h>
#include <Server.h>
#include <graphics.h>
#include <Snake.h>

void Socket_test(Network* net)
{
    net->Establish_Communication();
    net->Send("Hellow partner: please send a message\r\n");
    net->Recieve();
}


int main(int argc, char **argv)
{
	/*
	Client* cl = new Client("127.0.0.1");
	Socket_test(cl);
	delete cl;
    system("pause");
    Server* sr = new Server();
	Socket_test(sr);
	delete sr;
	*/
	Snake_entry_point(Right_Cuadrant,Play);
    return 0;
	
}
