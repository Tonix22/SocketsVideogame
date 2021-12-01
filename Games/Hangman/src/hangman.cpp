#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <hangman.h>

#define BGI_PATH "C:\\TURBOC3\\BGI"
std::string already_used;

int x_offset = 0;

void Draw_lines()
{
   int midx = getmaxx()/2;
   int midy = getmaxy()/2;
   // line for x1, y1, x2, y2
   line(midx, 0, midx, getmaxy());  
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
			state = lose;
			already_used.clear();
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
		line(a, 220, b, 220);		// letra
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

	if(*times >= size_word)
	{
		outtextxy(150+x_offset, 150, "WIN!");
		*times=0;
		already_used.clear();
		return win;
	}

	if(mode == Play)
	{
		ch = getch();
		arr[0]=ch;
		net->Send(arr);
		memset(arr,0,2);
	}
	else if (mode == Hold)
	{
		while(1)
		{
			net->Recieve();
			if(net->recvbuf[0]!=0){
				break;
			}
			delay(300);
		}
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

	if (already_used.find(ch) == std::string::npos) // not found
	{
		already_used.push_back(ch); // push
		*times+=(counter+1);
	}
	
	printf("times:%d\r\n",*times);
	
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


int hangman_game(Network* net, PlayMode state,MapLimits side){
	
	int gd    = DETECT,gm;
	int size_word=0, part=0;
	char *word;
	int ahorcado = ok;
	int corrects=0;
	int estado = ok;
	word = (char*)calloc(20,sizeof(char));
	bool finish_game = false;
	initgraph(&gd,&gm,BGI_PATH);
	Draw_lines();
	set_offset(side);
	if(state == Hold)
	{
		state = Transition;
	}

	while(finish_game == false)
	{
		if(state == Hold)
		{
			printf("Hold\r\n");
			draw_horca();			
			size_word = strlen(word);
			draw_underlines(size_word);
			corrects = 0;
			while(!ahorcado){
				estado = draw_char(word, &corrects, size_word,net,state);
				if(estado == fail){
					part++;
					ahorcado = draw_man(part,&corrects);
				}
				else if (estado == win){
					ahorcado = lose;
				}
			}
			cleardevice();
			ahorcado = ok;
			part = 1;
			state = Play;
		}
		else if(state == Transition)
		{
			printf("Transition\r\n");
			printf("Bienvenido ahorcado!!\n");
			printf("Ingresa Palabra\n");
			int i=0;
			do
			{
				word[i] = getch();
				printf("%c",word[i++]);
			}while(word[i-1]!='\n' && word[i-1]!='\r');
			net->Send(word);
			ahorcado=ok;
			state = Hold;
		}
		else if(state == Play)
		{
			corrects = 0;
			printf("Play\r\n");
			draw_horca();
			
			while(1)
			{
				net->Recieve();
				if(net->recvbuf[0]!=0){
					break;
				}
				delay(300);
			}
			strcpy(word,net->recvbuf);

			size_word = strlen(word);
			draw_underlines(size_word);

			printf("Palabra para adivinar: %s\r\n",word);
			
			while(!ahorcado){
				estado = draw_char(word, &corrects, size_word,net,state);
				if(estado == fail){
					part++;
					ahorcado = draw_man(part,&corrects);
				}
				else if (estado == win){
					ahorcado = lose;
				}
			}
			state = Transition;
			ahorcado = ok;
			part=1;
			cleardevice();
		}
	}
	closegraph();
   	return 0;
	
}

