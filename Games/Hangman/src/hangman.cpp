#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <hangman.h>

#define BGI_PATH "C:\\TURBOC3\\BGI"
std::string already_used[2];

int x_offset = 0;

void Non_Blocking_Recive(Network* net)
{
	while(1)
	{
		net->Recieve();
		if(net->recvbuf[0]!=0){
			break;
		}
		delay(300);
	}
}


void Draw_lines()
{
   int midx = getmaxx()/2;
   int midy = getmaxy()/2;
   // line for x1, y1, x2, y2
   line(midx, 0, midx, getmaxy()/2);  
   // line for x1, y1, x2, y2
   line(0, midy, getmaxx(), midy); 
}

/*
FUNCION DE DIBUJO DEL MONO AHORCADO
RETORNA TRUE SI DIBUJO 
RETORNA FALSE SI NO DIBUJO
*/
int draw_man(int part,int* times){
	
	int state = ok;
	
	switch(part){
		
		case HEAD:
			circle(100+x_offset,50,15);					//cabeza
			state = ok;
			break;
		case EYE_IZQ:
			ellipse(93+x_offset, 45, 0, 360, 1, 3); 		//ojo izq
			state = ok;
			break;
		case EYE_DER:
			ellipse(107+x_offset, 45, 0, 360, 1, 3); 	//ojo der
			state = ok;
			break;
		case BOCA:
			ellipse(100+x_offset,50,250,300,20,9); 		//boca
			state = ok;
			break;
		case BODY:
			ellipse(100+x_offset, 100, 0, 360, 15, 35); 	//cuerpo
			state = ok;
			break;
		case HAND_IZQ:
			line(115+x_offset, 80, 150+x_offset, 115);			// mano izq
			state = ok;
			break;
		case HAND_DER:
			line(85+x_offset, 80, 50+x_offset, 115);				// mano der
			state = ok;
			break;
		case FOOT_IZQ:
			line(115+x_offset, 125, 125+x_offset, 180);			// pie izq
			state = ok;
			break;
		case FOOT_DER:
			line(85+x_offset, 125, 75+x_offset, 180);				// pie der
			state = ok;
			break;
		default:
			outtextxy(150+x_offset, 150, "YOU LOSE!");
			outtextxy(100, 300, "Press any key to continue, or q to finish");
			state = lose;
			already_used[Server_instance].clear();
			already_used[Client_instance].clear();
			*times = 0;
			break;
	}
	return  state;
	
}

void draw_horca(){
	line(250+x_offset, 20, 250+x_offset, 215);		// poste 
	line(100+x_offset, 20, 250+x_offset, 20);			// viga
	line(200+x_offset, 215,300+x_offset, 215);		// soporte
	line(100+x_offset, 20, 100+x_offset, 40);			// soga
	line(235+x_offset, 20, 250+x_offset, 75);			// trave
}

void draw_underlines(int size){
	int i = 0;
	int a=32, b=40;
	for(i=0; i<size; i++){
		line(a+x_offset, 220, b+x_offset, 220);		// letra
		a+=10;
		b+=10;
	}
}

int draw_char(char *word,int* times,int size_word,Network* net,PlayMode mode){
	int pos_x = 32;
	char ch=0;
	char arr[2]= {};
	char *ret=NULL;
	char *token = NULL;
	std::string str_w2 = std::string(word);
	const char* word_2 = str_w2.c_str();
	int index = 0;
	printf("times:%d >= %d\r\n",*times,size_word);

	if(mode == Play)
	{
		ch = getch();
		arr[0]=ch;
		net->Send(arr);
		memset(arr,0,2);
	}
	else if (mode == Hold)
	{
		Non_Blocking_Recive(net);
		ch = net->recvbuf[0];
	}
	
	int counter = -1;
	//cacahuate
	//word_2 ->c
	//word_2 ->a  pos 1 acahuate index = 1
		//strchr->ret = 2dir
	char* incio = (char*)word_2;
	bool first_match = false;
	do{
		word_2 = word_2 + index;
		ret    = strchr(word_2, ch); //2dir
		//printf("ret: %s\r\n",ret);
		if(ret){
			index = (int)(ret-incio); //3
			//printf("acertada %d, indice %d\n", times,index);
			memcpy(arr, ret, 1);
		
			if(index <= size_word){
				if(index==0)	pos_x=32; // -> 0
				if(index>=1)	pos_x=32+(index*10);
				outtextxy(pos_x+x_offset, 215, arr);		
			}
			index ++; //1
			counter++;
		}
	}while(ret!=NULL);

	if(counter == -1)
	{
		printf("Incorrecto \n");
		return fail;
	}

	if (already_used[mode].find(ch) == std::string::npos) // not found
	{
		already_used[mode].push_back(ch); // push
		*times+=(counter+1);
	}
	if(*times >= size_word)
	{
		outtextxy(150+x_offset, 150, "WIN!");
		*times=0;
		already_used[Server_instance].clear();
		already_used[Client_instance].clear();
		outtextxy(100, 300, "Press any key to continue, or q to finish");
		return win;
	}

	return ok;
}

void set_offset(MapLimits side)
{
	if(side == Right_Cuadrant)
	{
		x_offset = 300;
	}
	else
	{
		x_offset = 0;
	}
}
void toggle_side(MapLimits* side)
{
	*side = (MapLimits)(*side^1);
	set_offset(*side);
}


int hangman_game(Network* net, PlayMode state,MapLimits side){
	
	int gd    = DETECT,gm;
	int size_word=0;

	bool finish_game = false;
	bool finish_match = false;

	char *word;
	int ahorcado = ok;
	int estado   = ok;

	Score hits      = {0,0};
	Score penalties = {0,0};

	PlayMode original = state;

	word = (char*)calloc(20,sizeof(char));
	srand(time(0));

	initgraph(&gd,&gm,BGI_PATH);
	
	while(finish_game == false)
	{
		Draw_lines();
		if(net->role == Server_instance)
		{
			std::string pick = get_random_word();
			strcpy(word,pick.c_str());
			net->Send(pick);
		}
		if(net->role == Client_instance)
		{
			Non_Blocking_Recive(net);
			strcpy(word,net->recvbuf);
			toggle_side(&side);
		}

		size_word = strlen(word);
		
		for(int cnt=0; cnt < 2; cnt++)
		{
			draw_horca();
			draw_underlines(size_word);
			toggle_side(&side);
		}
		while(finish_match == false)
		{
			int* current_penal;
			int* current_hit;
			if(state == Play)
			{
				current_penal = &(penalties.Me);
				current_hit   = &(hits.Me);
			}
			if(state == Hold)
			{
				current_penal = &(penalties.You);
				current_hit   = &(hits.You);
				toggle_side(&side);
			}

			estado = draw_char(word, current_hit, size_word,net,state);
			if(estado == fail){
				(*current_penal)++;
				ahorcado = draw_man(*current_penal,current_hit);
				if(ahorcado == lose)
				{
					goto clean;
				}
			}
			else if (estado == win){
				clean:
				memset(&hits,     0,sizeof(Score));
				memset(&penalties,0,sizeof(Score));
				finish_match = true;
			}
			if(state == Hold)
			{
				toggle_side(&side);
			}
			state = (PlayMode)(state^1);
		}
		finish_game  = (getch()=='q')?true:false;
		finish_match = false;
		state = original;
		cleardevice();
	}
	closegraph();
   	return 0;
	
}

