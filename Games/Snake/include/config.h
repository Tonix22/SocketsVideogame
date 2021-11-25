#ifndef CONFIG_H
#define CONFIG_H

#define false 0
#define true 1

typedef enum vect
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
}Direction;

typedef struct pair
{
   int x;
   int y;
}Coord;

typedef struct body
{
   Coord pos;
}Snake;

typedef struct rumble
{
   Coord top_left;
   Coord top_right;
   Coord buttom_right;
   Coord buttom_left;
}Limits;

int getRandrange(int lower, int upper);
void Draw_apple(int x,int y,bool erase);
void Place_Apple();
void Place_Snake_Start(Snake* head);
void Draw_Snake(Snake* head);
void Update_Snake_pos(int x,int y);
void Draw_Grid();

#endif
