/*
 * globalVars.h
 *
 *  Created on: Aug 21, 2014
 *      Author: herve
 */

#ifndef GLOBALVARS_H_
#define GLOBALVARS_H_

//include bibliothéque SDL
#include <SDL/SDL.h>
//#include <SDL/SDL_image.h>
//#include <SDL/SDL_mixer.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_ttf.h>


//standard C++ bib
#include <iostream>
#include <string>
#include <sstream>

//specifique C (pour fct
#include <unistd.h>

//Grille du jeu
extern int tabNum[4][4];

//not used
//extern int FRAME_PER_SECOND;

//booleen pour la sortie de la boucle de jeu...
extern bool GoOut;

//surface principale et evenements principaux...
extern SDL_Surface* screen;
extern SDL_Event event;

#endif /* GLOBALVARS_H_ */
