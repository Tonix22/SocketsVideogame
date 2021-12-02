#ifndef HANGMAN_H
#define HANGMAN_H

#include "GameManager.h"
#include "Network.h"

enum el_man{
	HEAD = 0,
	EYE_IZQ,
	EYE_DER,
	BOCA,
	BODY,
	HAND_IZQ,
	HAND_DER,
	FOOT_IZQ,
	FOOT_DER,
	CUERPO
};

enum estado{
	ok,
	lose,
	win,
	fail
};
typedef struct Points
{
	int Me;
	int You;
}Score;

int hangman_game(Network* net, PlayMode state,MapLimits side);

#endif
