/*
 * main.cpp
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


//include local application
#include "AppGame.h"

using namespace game;

AppGame* jeu;

int main (int argc, char ** argv){
//centrer la fenêtre
putenv("SDL_VIDEO_CENTERED=1");


//creer et initialiser le jeu
jeu=new game::AppGame();


if (!jeu->initSDL()){return 1;}

jeu->loopGame();

delete(jeu);
//quitter
return 0;
}

