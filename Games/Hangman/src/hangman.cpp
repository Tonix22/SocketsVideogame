#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <hangman.h>

#define BGI_PATH "C:\\TURBOC3\\BGI"
/*
void Draw_lines()
{
   int midx = getmaxx()/2;
   int midy = getmaxy()/2;
   // line for x1, y1, x2, y2
   line(midx, 0, midx, getmaxy());  
   // line for x1, y1, x2, y2
   line(0, midy, getmaxx(), midy); 
}
*/
/*
FUNCION DE DIBUJO DEL MONO AHORCADO
RETORNA TRUE SI DIBUJO 
RETORNA FALSE SI NO DIBUJO
*/
int draw_man(int part){
	
	int state = ok;
	
	switch(part){
		
		case HEAD:
			circle(100,50,15);					//cabeza
			state = ok;
			break;
		case EYE_IZQ:
			ellipse(93, 45, 0, 360, 1, 3); 		//ojo izq
			state = ok;
			break;
		case EYE_DER:
			ellipse(107, 45, 0, 360, 1, 3); 	//ojo der
			state = ok;
			break;
		case BOCA:
			ellipse(100,50,250,300,20,9); 		//boca
			state = ok;
			break;
		case BODY:
			ellipse(100, 100, 0, 360, 15, 35); 	//cuerpo
			state = ok;
			break;
		case HAND_IZQ:
			line(115, 80, 150, 115);			// mano izq
			state = ok;
			break;
		case HAND_DER:
			line(85, 80, 50, 115);				// mano der
			state = ok;
			break;
		case FOOT_IZQ:
			line(115, 125, 125, 180);			// pie izq
			state = ok;
			break;
		case FOOT_DER:
			line(85, 125, 75, 180);				// pie der
			state = ok;
			break;
		default:
			outtextxy(150, 150, "YOU LOSE!");
			state = lose;
			break;
	}
	return  state;
	
}

void draw_horca(){
	line(250, 20, 250, 215);		// poste 
	line(100, 20, 250, 20);			// viga
	line(200, 215, 300, 215);		// soporte
	line(100, 20, 100, 40);			// soga
	line(235, 20, 250, 75);			// trave
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

int draw_char(char *word,int times,int size_word){
	int pos_x = 32;
	char ch=NULL;
	char arr[2]= {};
	char *ret=NULL;
	char *token = NULL;
	int index =0;
	printf("Ingresa Letra:\n");
	ch = getch();
	ret=strchr(word, ch);
	if(ret){
		index = (int)(ret-word);
		printf("acertada %d, indice %d\n", times,index);
	}
	else{
		printf("Incorrecto \n");
		return fail;
	}

	
	memcpy(arr, ret, 1);
	//token = strtok(word,arr);
	
	if(times==size_word){;
		outtextxy(150, 150, "WIN!");
		return win;
	}
	else if(index <= size_word){
		if(index==0)	pos_x=32;
		if(index>=1)	pos_x+=(index*10);
		
		outtextxy(pos_x, 215, arr);
	}

	
	
	return ok;
}

int hangman_game(Network* net, PlayMode state){
	
	int gd    = DETECT,gm;
	int size_word=0, part=0;
	char *word;
	int ahorcado = ok;
	int corrects=0;
	int estado = ok;
	word = (char*)malloc( 20*sizeof(char));
	bool finish_game = false;
	initgraph(&gd,&gm,BGI_PATH);
	if(state == Play)
	{
		state = Transition;
	}

	while(finish_game == false)
	{
		if(state == Hold)
		{
			net->Recieve();
			while(strcmp(net->recvbuf,"Fail")==0 || strcmp(net->recvbuf,"Good") == 0)
			{
				net->Recieve();
				printf("%s\r\n",net->recvbuf);
			}
			strcpy(word,net->recvbuf);
			state = Play;
		}
		else if(state == Transition)
		{
			printf("Bienvenido ahorcado!!\n");
			printf("Ingresa Palabra\n");
			gets(word);
			net->Send(word);
			ahorcado=0;
			state = Hold;
		}
		else if(state == Play)
		{
			draw_horca();
			size_word = strlen(word);
			draw_underlines(size_word);
			
			while(!ahorcado){
				estado = draw_char(word, corrects, size_word);
				if(estado == fail){
					part++;
					ahorcado = draw_man(part);
					net->Send("Fail");
				}
				else if(estado == ok ){
					corrects++;
					net->Send("Good");
				}
				else if (estado == win){
					ahorcado = lose;
				}
			}
			state = Transition;
			cleardevice();
		}
	}
	closegraph();
   	return 0;
	
}

