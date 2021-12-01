#include <Client.h>
#include <Server.h>
#include <graphics.h>
#include <Snake.h>
#include <hangman.h>

#define SERVER (1)

void Main_program(Network* net)
{
    net->Establish_Communication();
    //Snake_entry_point(net,net->cuadrant,net->mode);
    hangman_game(net,net->mode,net->cuadrant);
}


int main(int argc, char **argv)
{
	#if SERVER
	Server* sr = new Server();
	Main_program(sr);
	#else
	Client* cl = new Client("127.0.0.1");
	Main_program(cl);
	#endif

    return 0;
	
}
