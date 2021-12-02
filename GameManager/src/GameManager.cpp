#include "GameManager.h"
#include <stdlib.h>

std::string words[WORDS_LEN] = {"perro",
"gato",
"ejemplo",  
"semana",  
"varios",  
"real", 
"servicios",  
"haya",  
"principio",  
"encuentra",   
"gracias",  
"ministerio",  
"viejo",  
"encima",   
"resulta",  
"pequeño",  
"fecha",
"instituciones",  
"temas",  
"militares",  
"hospital",
"amigos", 
"materia",
"llega", 
"carrera",
"mes",
"pronto",
"soy",
"cine",   
"salir",
"comunicacion",
"compa",
"amigo",   
"autoridades",
"creacion",
"chile",
"precio", 
"posibilidades",
"centros",
"profesional",
"mexico",
"espana",   
"condiciones"};


int getRandrange(int lower, int upper)
{
   return (rand() %(upper - lower + 1)) + lower;
}
std::string get_random_word()
{
    return words[getRandrange(0,WORDS_LEN-1)];
}