/*
 * globalVars.h
 *
 *  Created on: Aug 21, 2014
 *      Author: herve
 */

#ifndef GLOBALVARS_H_
#define GLOBALVARS_H_

//include SDL bib
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_ttf.h>


//standard C++ bib
#include <iostream>
#include <string>
#include <sstream>

//specifique C
#include <unistd.h>


extern int tabNum[4][4];
extern int tabNumTmp[4][4];

extern int FRAME_PER_SECOND;

extern bool GoOut;

extern SDL_Surface* screen;
extern SDL_Event event;

#endif /* GLOBALVARS_H_ */
