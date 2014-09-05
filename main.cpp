/*
 * main.cpp
 *
 *  Created on: Aug 21, 2014
 *      Author: herve
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

