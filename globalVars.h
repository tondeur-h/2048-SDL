/*
 * globalVars.h
 *
 *  Created on: 21 Aout 2014
 *      Author: Tondeur Herve
 *
 *  This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef GLOBALVARS_H_
#define GLOBALVARS_H_

//include SDL bib
#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_ttf.h>


//standard C++ bib
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <ctime>
#include <fstream>


extern int tabNum[4][4];

extern int FRAME_PER_SECOND;

extern bool GoOut;

extern SDL_Surface* screen;
extern SDL_Event event;

#endif /* GLOBALVARS_H_ */
