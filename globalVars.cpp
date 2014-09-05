/*
 * globalVars.cpp
 *
 *  Created on: Aug 21, 2014
 *      Author: herve
 */

#include "globalVars.h"

//ma grille de jeu...
int tabNum[12][12];

//surface et evenement principaux
SDL_Surface* screen=NULL;
SDL_Event event;

// fin de boucle
bool GoOut=false;

//
int gameOver=false;
int gameWin=false;
long int score=0;
long int bestScore=0;
bool boolChal=false;
int GOAL=2048;
int hit=0;

//Taille de la grille
int nbCase=4;
