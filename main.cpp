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

//init SDL
if (!jeu->initSDL()){std::cout<<"Erreur initialisation SDL!"<<std::endl;return 1;}

//boucle du jeu
jeu->loopGame();

delete(jeu);
//quitter
return 0;
}

