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
	//Server* sr = new Server();
    //sr->Establish_Communication();
	//Snake_entry_point(sr,Left_Cuadrant,Play);
	
	Client* cl = new Client("127.0.0.1");
	cl->Establish_Communication();
	Snake_entry_point(cl,Right_Cuadrant,Hold);
	
	//Socket_test(sr);
	/*

	*/
    return 0;
	
}
