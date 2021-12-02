#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include <time.h>
#include <string>

#define WORDS_LEN 43


typedef enum
{
    Play,
    Hold,
    Transition
}PlayMode;

typedef enum 
{
    Right_Cuadrant,
    Left_Cuadrant,
}MapLimits;

int getRandrange(int lower, int upper);
std::string get_random_word();

#endif