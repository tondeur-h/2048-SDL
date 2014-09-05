/*
 * globalVars.h
 *
 *  Created on: Aug 21, 2014
 *      Author: herve
 */

#ifndef GLOBALVARS_H_
#define GLOBALVARS_H_

//include SDL bib
#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <SDL_image.h>


//standard C++ bib
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <ctime>
#include <fstream>

//standard C lib
#include <stdlib.h>

//variables globales
extern int tabNum[12][12];

extern bool GoOut;

extern int gameOver;
extern int gameWin;
extern long int score;
extern long int bestScore;
extern bool boolChal;
extern int GOAL;
extern int hit;

//Taille de la grille
extern int nbCase;

extern SDL_Surface* screen;
extern SDL_Event event;

#endif /* GLOBALVARS_H_ */
