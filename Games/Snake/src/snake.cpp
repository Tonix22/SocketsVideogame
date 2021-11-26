#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "config.h"
#include "snake.h"


//Linux
//gcc test.c -lgraph

#define BGI_PATH "C:\\TURBOC3\\BGI"
// must be even
#define APPLE_SIZE 12 
#define SNAKE_SIZE 12
#define MAX_SNAKE_SIZE 64

#ifdef ARROWS
   #define KEYUP 72 
   #define KEYDOWN 80
   #define KEYLEFT 75 
   #define KEYRIGHT 77
   #define KEYESC   27
#else
   #define KEYUP 'w' 
   #define KEYDOWN 's'
   #define KEYLEFT 'a' 
   #define KEYRIGHT 'd'
   #define KEYESC   27
#endif

Snake snk[MAX_SNAKE_SIZE]={0};
Coord last[MAX_SNAKE_SIZE];
Limits wall;
int head = 0;

/************************************
*********** COMMON *******************
*************************************/

int getRandrange(int lower, int upper)
{
   return (rand() %(upper - lower + 1)) + lower;
}
void Errase_Section(int x1, int y1,int x2,int y2)
{ 
   int poly[10];   /* our polygon array */
   /* first vertex */
   poly[0] = x1;
   poly[1] = y1;
   /* second vertex */
   poly[2] = x2;    
   poly[3] = poly[1];
   /* third vertex */
   poly[4] = poly[2];    
   poly[5] = y2;
   /* fourth vertex */
   poly[6] = poly[0];     
   poly[7] = poly[5];
   /* drawpoly doesn't automatically close */
   /* the polygon, so we close it */
   poly[8] = poly[0];
   poly[9] = poly[1];
   /* draw the polygon */
   setcolor(BLACK);
   setfillstyle(SOLID_FILL, BLACK);
   fillpoly(5, poly);
   setcolor(WHITE);
}

/************************************
*********** APPLE *******************
*************************************/
  
void Draw_apple(int x,int y,bool erase)
{
   int poly[10];   /* our polygon array */
   Coord upper_left_corner;
   Coord down_right_corner;
   upper_left_corner.x = x-APPLE_SIZE/2;
   upper_left_corner.y = y-APPLE_SIZE/2;
   down_right_corner.x = x+APPLE_SIZE/2;
   down_right_corner.y = y+APPLE_SIZE/2;

   /* first vertex */
   poly[0] = upper_left_corner.x;
   poly[1] = upper_left_corner.y;
   /* second vertex */
   poly[2] = poly[0]+APPLE_SIZE;    
   poly[3] = poly[1];
   /* third vertex */
   poly[4] = down_right_corner.x;    
   poly[5] = down_right_corner.y;
   /* fourth vertex */
   poly[6] = poly[4]-APPLE_SIZE;     
   poly[7] = down_right_corner.y;
   /* drawpoly doesn't automatically close */
   /* the polygon, so we close it */
   poly[8] = poly[0];
   poly[9] = poly[1];
   /* draw the polygon */
   if(erase)
   {
      setcolor(BLACK);
      setfillstyle(SOLID_FILL, BLACK);
   }
   else{
      setcolor(RED);
      setfillstyle(SOLID_FILL, RED);
   }
   fillpoly(5, poly);
   setcolor(WHITE);
}
void Place_Apple(Coord* apple)
{
   if(apple->x !=0 && apple->y!=0)
   {
      Errase_Section(wall.top_left.x+3,wall.top_left.y+3,wall.buttom_right.x-3,wall.buttom_right.y-3);
   }

   apple->x = getRandrange(wall.top_left.x  + APPLE_SIZE,
                           wall.top_right.x - APPLE_SIZE);

   apple->y = getRandrange(wall.top_left.y    + APPLE_SIZE,
                           wall.buttom_left.y - APPLE_SIZE);
  
   Draw_apple(apple->x,apple->y,false);
}
bool CollideApple(Coord snake)
{
   int i,j;
   
   for (j = snake.y-SNAKE_SIZE; j < snake.y+SNAKE_SIZE; j++)
   {
      for (i = snake.x-SNAKE_SIZE; i < snake.x+SNAKE_SIZE; i++)
      {
         if(getpixel(i,j) == RED)
         {
            return true;
         }
      }
   }
   return false;
}


/************************************
*********** SNAKE *******************
*************************************/
  
int isfull() {

   if(head == MAX_SNAKE_SIZE)
      return 1;
   else
      return 0;
}

void push_snake() {

   if(!isfull()) {
      head++;
      snk[head].pos.x = snk[head-1].pos.x;
      snk[head].pos.y = snk[head-1].pos.y;
      
   } else {
      printf("YOU WIN!!\n");
   }
}

void Place_Snake_Start(Snake* body)
{
   if(head == 0)
   {
      body->pos.x = getRandrange(SNAKE_SIZE + wall.top_left.x,
                                 wall.top_right.x-APPLE_SIZE);

      body->pos.y = getRandrange(APPLE_SIZE + wall.top_left.y,
                                 wall.buttom_left.y-APPLE_SIZE);
   }
}
void Draw_Snake(Snake* body)
{
   rectangle(body->pos.x-SNAKE_SIZE/2,body->pos.y-SNAKE_SIZE/2, 
             body->pos.x+SNAKE_SIZE/2,body->pos.y+SNAKE_SIZE/2);
}

void Errase_Snake(int idx)
{
   setcolor(getbkcolor());
   Draw_Snake(&(snk[idx]));
   setcolor(WHITE);
}

void Erasse_All_Snake()
{
   int i=0;
   for (i = 0; i <= head; i++)
   {
      Errase_Snake(i);
   }
   head = 0;
   snk[0].pos.x = last->x;
   snk[0].pos.y = last->y;
}

bool Snake_Collide_Wall()
{  
   if((snk[head].pos.x-(SNAKE_SIZE+2)) < wall.top_left.x  || 
      (snk[head].pos.y-(SNAKE_SIZE+2)) < wall.top_left.y ||
      (snk[head].pos.x+(SNAKE_SIZE+2)) > wall.buttom_right.x || 
      (snk[head].pos.y+(SNAKE_SIZE+2)) > wall.buttom_right.y)
   {
      return true;
   }
   return false;
}

bool Snake_Collide_MiddleWall(MapLimits side)
{
   if(side == Right_Cuadrant)
   {
      if((snk[0].pos.x-(SNAKE_SIZE+3)) < wall.buttom_left.x)
      {
         return true;
      }
   }
   else if(side == Left_Cuadrant)
   {
      if((snk[0].pos.x+(SNAKE_SIZE+3)) > wall.buttom_right.x)
      {
         return true;
      }
   }
   return false;
}

bool Snake_Collide_Snake()
{
   int i=0;
   if(head > 3)
   {
      for(i=0;i<head-3;i++)
      {
         if( (snk[i].pos.x-SNAKE_SIZE/2) < snk[head].pos.x   &&
              snk[head].pos.x < (snk[i].pos.x+SNAKE_SIZE/2) &&
             (snk[i].pos.y-SNAKE_SIZE/2) < snk[head].pos.y &&
              snk[head].pos.y < (snk[i].pos.y+SNAKE_SIZE/2))
         {
            return true;
         }
      }
   }
   return false;
}


void Update_Snake_pos(int x,int y)
{
   int j=0;
   int i=head;
   for (i = head; i > -1; i--,j++)
   {
      //Errase
      Errase_Snake(i);
      last[j].x = snk[i].pos.x;
      last[j].y = snk[i].pos.y;
      if(i == head)
      {
         snk[head].pos.x = x;
         snk[head].pos.y = y;
      }else
      {
         snk[i].pos.x = last[j-1].x;
         snk[i].pos.y = last[j-1].y;
      }
      //Draw again
      Draw_Snake(&(snk[i]));
   }
}

void Draw_Grid()
{
   int midx = getmaxx()/2;
   int midy = getmaxy()/2;
   // line for x1, y1, x2, y2
   line(midx, 0, midx, getmaxy());  
   // line for x1, y1, x2, y2
   line(0, midy, getmaxx(), midy); 
}
void Set_Wall_Limits(MapLimits lim)
{
   if(lim == Left_Cuadrant)
   {
      wall.top_left.x = 0;
      wall.top_left.y = 0;

      wall.top_right.x = getmaxx()/2;
      wall.top_right.y = wall.top_left.y;

      wall.buttom_right.x = wall.top_right.x;
      wall.buttom_right.y = getmaxy()/2;

      wall.buttom_left.x = wall.top_left.x;
      wall.buttom_left.y = wall.buttom_right.y;
   }
   else
   {
      wall.top_left.x = getmaxx()/2;
      wall.top_left.y =  wall.top_left.y;

      wall.top_right.x = getmaxx();
      wall.top_right.y = wall.top_left.y;

      wall.buttom_right.x = wall.top_right.x;
      wall.buttom_right.y = getmaxy()/2;

      wall.buttom_left.x = wall.top_left.x;
      wall.buttom_left.y = wall.buttom_right.y;
   }
}
void get_key(int*key, int*x,int*y,int step)
{
   if(kbhit())
   {
      *key = getch();
   }
   if(*key==KEYUP)
   {
      (*y)-=step;
   }
   if(*key == KEYLEFT)
   {
      (*x)-=step;
   }
   if(*key == KEYDOWN)
   {
      (*y)+=step;
   }
   if(*key == KEYRIGHT)
   {
      (*x)+=step;
   }
}


int Snake_entry_point(MapLimits side, PlayMode state)
{
   int i, j  = 0;
   int gd    = DETECT,gm; 
   int step  = SNAKE_SIZE;
   int cmd  = 0;
   bool fts=true;
   Coord apple;
   apple.x = 0;
   apple.y = 0;
   srand(time(0));
   
   #ifdef __linux__ 
   initgraph (& gd,& gm,NULL);
   #else
   initgraph(&gd,&gm,BGI_PATH);
   #endif

   Draw_Grid();

   Set_Wall_Limits(side);
   //Dependes on the role
   Place_Snake_Start(&(snk[head]));

   i = snk[head].pos.x ;
   j = snk[head].pos.y ;
   
   while(1)
   {
      get_key(&cmd,&i,&j,step);
      if(cmd == KEYESC)
      {
         break;
      }
      if(cmd!= 0)
      {
         if(state == Play)
         {
            if(fts == true)
            {
               Place_Apple(&apple);
               fts = false;
            }
            if(Snake_Collide_Wall())
            {
               Erasse_All_Snake();
               cmd = getch();
               get_key(&cmd,&i,&j,step*2);
               Update_Snake_pos(i,j);
               Place_Apple(&apple);
            }
            else if (Snake_Collide_Snake())
            {
               Erasse_All_Snake();
               getch();
               cmd = 0;
               Place_Apple(&apple);
               Place_Snake_Start(&(snk[head]));
            }
            
            else if(CollideApple(snk[head].pos))
            {
               Errase_Section(wall.top_left.x+3,wall.top_left.y+3,wall.buttom_right.x-3,wall.buttom_right.y-3);
               state = Transition;
               push_snake();
               Update_Snake_pos(i,j);
            }
            else
            {
               Update_Snake_pos(i,j);
            }
         }
         if(state == Transition)
         {
            if(Snake_Collide_MiddleWall(side) == true)
            {
               int snk_size = head;
               for (int i = snk_size; i > -1; i--)
               {
                  Errase_Snake(i);
                  head--;
                  delay(150);
               }
               head = 0;
               snk[0].pos.x = last->x;
               snk[0].pos.y = last->y;
               state = Hold;     
            }
            else
            {
               Update_Snake_pos(i,j);
            }
         }
         if(state == Hold)
         {
            //when hold ends
            //fts = true;
         }
      }
      delay(150);
   }
   closegraph();
   
   return 0;
}
