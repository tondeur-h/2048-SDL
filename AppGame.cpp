/*
 * AppGame.cpp
 *
 *  Created on: Aug 22, 2014
 *      Author: herve
 */

#include "AppGame.h"

namespace game {

//const pour le mode video
const int SCREENWIDTH=480;
const int SCREENHEIGHT=640;
const int SCREENBPP=0; //profondeur max dispo

//tick pour le calcul du frame rate
//long int firstTick, lastTick;

//coordonnées de la souris
int X,Y;

//coordonnées grille
int ep=2;
int left=40;
int top=200;
int width=400;
int height=400;
int nbCase=4;
//calcul des pas d'espacement des cases
int stepW=width/nbCase;
int stepH=height/nbCase;

//quelques variables utiles
int direction;
long int score;
int resultCheck;  //valeur de retour du contrôle de la grille de jeu
int gameOver=false;
int gameWin=false;

//pour l'affichage
TTF_Font* fontLarge;
TTF_Font* fontMedium;
TTF_Font* fontSmall;
TTF_Font* fontVerySmall;

SDL_Color fgColor;

SDL_Rect rectDst;

SDL_Surface* renderNumber;
SDL_Surface* renderScore;
SDL_Surface* renderNewGame;
SDL_Surface* renderQuitGame;
SDL_Surface* renderCopyright;
SDL_Surface* renderGameOver;
SDL_Surface* renderGameWin;

/**************
 * nouveau a la creation de l'objet.
 **************/
AppGame::AppGame() {
	newGame();
}




/****************
 * concat current directory with relatif path and file name...
 *****************/
std::string AppGame::get_file(std::string file){
return std::string(get_current_dir_name())+file;
}




/*****************
 * Initialise SDL et le mode video
 *****************/
bool AppGame::initSDL(){
	if (SDL_Init(SDL_INIT_EVERYTHING)==-1){
		std::cout<<"Erreur init SDL!"<<std::endl;
		return false;
	}//if

	//mode fenêtre
	if ((screen=SDL_SetVideoMode(SCREENWIDTH,SCREENHEIGHT,SCREENBPP,SDL_SWSURFACE))==NULL){
		std::cout<<"Erreur set Video SDL!"<<std::endl;
				return false;
	}//if

	SDL_WM_SetCaption("2p11 Game version 1.0",NULL);

	if (TTF_Init()==-1){
		std::cout<<"Erreur loading font!"<<std::endl;
			return false;
	}//if

	fontLarge=TTF_OpenFont(get_file("/gfx/font.fnt").c_str(),58);
	fontMedium=TTF_OpenFont(get_file("/gfx/font.fnt").c_str(),38);
	fontSmall=TTF_OpenFont(get_file("/gfx/font.fnt").c_str(),28);
	fontVerySmall=TTF_OpenFont(get_file("/gfx/font.fnt").c_str(),14);

	return true;
}




/**
 * calculer le temp d'attente entre chaque Frame
 * impose le frame rate (non utilisé)
 */
/*void AppGame::frameRate(){
lastTick=SDL_GetTicks();
long int spent=lastTick-firstTick;
long int TIME_FRAME=(1000/FRAME_PER_SECOND);
if (spent>TIME_FRAME) { spent=TIME_FRAME;}
	SDL_Delay(TIME_FRAME-spent);
}*/




/**
 * Boucle principale du jeu
 */
void AppGame::loopGame(){

	//ignorer les evenement du clavier
	SDL_EventState(SDL_KEYDOWN,SDL_IGNORE);
	SDL_EventState(SDL_KEYUP,SDL_IGNORE);

	//dessine le plateau une premiere fois
		drawTab();

	while (!GoOut){

		if (SDL_WaitEvent(&event)==1){
		//controle clavier et souris
		switch(event.type){

		case SDL_QUIT:
						GoOut=true;
						break;

		case SDL_MOUSEBUTTONDOWN:
						if (event.button.button==SDL_BUTTON_LEFT){
							X=event.button.x;
							Y=event.button.y;
							direction=mouseConvertPos(X,Y);
							if (direction==5) {newGame();drawTab();direction=0;}
							if (direction==6) {GoOut=true;direction=0;}
							if (direction>=1 && direction<=4)
							{
																calculateTab(direction);
																resultCheck=checkTab();
																direction=0;
																if (resultCheck==0){gameOver=true;}
																if (resultCheck==3){gameWin=true;}
							}

						}
						break;

		} //switch

			//dessine le plateau
			drawTab();

	} //if

	}//while
}




/*******************
 * Verifier si le tableau n'est pas complet.
 *******************/
int AppGame::checkTab(){
bool compactable=false;
bool OK2048=false;
int nbVide=0;
/* valeurs de retour
 * 0 fin de jeu
 * 1 nouveau nombre
 * 2 plus de case mais compactable
 * 3 2048 atteint winner
 */

//---------------------------------------------------------------------
//calculer nombre de caseVide
for (int x=0;x<4;x++){
	for (int y=0;y<4;y++){
		if (tabNum[x][y]==0) {nbVide++;}
	}
}

//si nbVide>1
if (nbVide>1){
//ajouter un nombre 2 ou 4 et quitter
	int nbOK=0;
	while(nbOK<1){
		int x=rand()%4;int y=rand()%4;
		if (tabNum[x][y]==0) {tabNum[x][y]=pow(2,(rand()%2)+1); nbOK++;}
	} //while
return 1;
}
else
{
	//ajouter un nombre 2 ou 4
		int nbOK=0;
		while(nbOK<1){
			int x=rand()%4;int y=rand()%4;
			if (tabNum[x][y]==0) {tabNum[x][y]=pow(2,(rand()%2)+1); nbOK++;}
		} //while
}

//---------------------------------------------------------------------
//Tester si compactable doit y avoir 2 valeurs identiques contigue
//tester les lignes
for (int y=0;y<4;y++){
	for (int x=0;x<3;x++){
		if (tabNum[x+1][y]==tabNum[x][y]) {compactable=true;break;}
	}
}
//tester les colonnes
for (int x=0;x<4;x++){
	for (int y=0;y<3;y++){
		if (tabNum[x][y+1]==tabNum[x][y]) {compactable=true;break;}
	}
}
//si OK  sortir on peut continuer
if (compactable) return 2;

//---------------------------------------------------------------------
//tester si 2048 atteint => the winner
for (int x=0;x<4;x++){
	for (int y=0;y<4;y++){
		if (tabNum[x][y]==2048) {OK2048=true;break;}
	}
}
//si 2048 atteint fin du jeu...
if (OK2048) return 3;

//---------------------------------------------------------------------
//fin de jeu, plus aucun coup possible ;-(
return 0;
}




/**
 * calcule les sommes du tableau aprés un deplacement
 */
void AppGame::calculateTab(int dir){
int z=0;
/*
 * 1 vers le haut
 * 2 vers la droite
 * 3 vers le bas
 * 4 vers la gauche
 */

if (dir==1){
	//compactages des nombres
	for (int x=0;x<4;x++){
		for (int y=1;y<4;y++){
			if (tabNum[x][y]>0){z=y;}
			while(z-1>=0){
				if (tabNum[x][z-1]==0){tabNum[x][z-1]=tabNum[x][z];tabNum[x][z]=0;}
				z--;
			}//while
		}//for y
	}//for x


	//calcul des sommes
	for (int x=0;x<4;x++){
			for (int y=0;y<4;y++){
				if (tabNum[x][y+1]==tabNum[x][y]){
								tabNum[x][y]=tabNum[x][y]*2;score=score+tabNum[x][y];tabNum[x][y+1]=0;
							}
			}//for y
		}//for x


//second déplacement
for (int x=0;x<4;x++){
//pour x=1 a x<4
			//si (x)>0 alors z=x //fin si
			//tant que z-1>0
				//si (z-1)=0 alors (z-1)=(z) et (z)=(0)
				//z--
			//fin tant que
		//fin pour

	for (int y=1;y<4;y++){
		if (tabNum[x][y]>0){z=y;}
		while(z-1>=0){
			if (tabNum[x][z-1]==0){tabNum[x][z-1]=tabNum[x][z];tabNum[x][z]=0;}
			z--;
		}//while
	}//for y
}//for x
}//if

if (dir==2){
	//compactage des nombres
		for (int y=0;y<4;y++){
			for (int x=2;x>=0;x--){
				if (tabNum[x][y]>0){z=x;}
				while(z+1<=3){
					if (tabNum[z+1][y]==0){tabNum[z+1][y]=tabNum[z][y];tabNum[z][y]=0;}
					z++;
				}//while
			}//for x
		}//for y

	//calculer les sommes
		for (int y=0;y<4;y++){
			for (int x=0;x<3;x++){
				if (tabNum[x+1][y]==tabNum[x][y]){
					tabNum[x][y]=tabNum[x][y]*2;score=score+tabNum[x][y];tabNum[x+1][y]=0;
				}
			}//for x
		}//for y


	//deplacer les elelements
	for (int y=0;y<4;y++){
		for (int x=2;x>=0;x--){
			if (tabNum[x][y]>0){z=x;}
			while(z+1<=3){
				if (tabNum[z+1][y]==0){tabNum[z+1][y]=tabNum[z][y];tabNum[z][y]=0;}
				z++;
			}//while
		}//for x
	}//for y
	}//if

if (dir==3){

	//compactage des nombres
	//second deplacement
		for (int x=0;x<4;x++){
				for (int y=2;y>=0;y--){
					if (tabNum[x][y]>0){z=y;}
					while(z+1<=3){
						if (tabNum[x][z+1]==0){tabNum[x][z+1]=tabNum[x][z];tabNum[x][z]=0;}
						z++;
					}//while
				}//for y
			}//for x

	//calcul des sommes
		for (int x=0;x<4;x++){
				for (int y=3;y>=1;y--){
					if (tabNum[x][y-1]==tabNum[x][y]){
									tabNum[x][y]=tabNum[x][y]*2;score=score+tabNum[x][y];tabNum[x][y-1]=0;
								}
				}//for y
			}//for x

	//second deplacement
	for (int x=0;x<4;x++){
			for (int y=2;y>=0;y--){
				if (tabNum[x][y]>0){z=y;}
				while(z+1<=3){
					if (tabNum[x][z+1]==0){tabNum[x][z+1]=tabNum[x][z];tabNum[x][z]=0;}
					z++;
				}//while
			}//for y
		}//for x
}

if (dir==4){
	//compactage des valeurs
	for (int y=0;y<4;y++){
		for (int x=1;x<4;x++){
			if (tabNum[x][y]>0){z=x;}
			while(z-1>=0){
				if (tabNum[z-1][y]==0){tabNum[z-1][y]=tabNum[z][y];tabNum[z][y]=0;}
				z--;
			}//while
		}//for x
	}//for y

//calculer les sommes
	for (int y=0;y<4;y++){
		for (int x=3;x>=1;x--){
			if (tabNum[x-1][y]==tabNum[x][y]){
				tabNum[x][y]=tabNum[x][y]*2;score=score+tabNum[x][y];tabNum[x-1][y]=0;
			}
		}//for x
	}//for y

//second déplacement
for (int y=0;y<4;y++){
//pour x=1 a x<4
			//si (x)>0 alors z=x //fin si
			//tant que z-1>0
				//si (z-1)=0 alors (z-1)=(z) et (z)=(0)
				//z--
			//fin tant que
		//fin pour

	for (int x=1;x<4;x++){
		if (tabNum[x][y]>0){z=x;}
		while(z-1>=0){
			if (tabNum[z-1][y]==0){tabNum[z-1][y]=tabNum[z][y];tabNum[z][y]=0;}
			z--;
		}//while
	}//for x
}//for y
}//if

}




/*********************
 * dessine le tableau et les valeurs du tableau
 *********************/
void AppGame::drawTab(){
//vider l'écran
boxRGBA(screen,0,0,SCREENWIDTH,SCREENHEIGHT,0,0,0,255);
drawGrid();
drawTools();
drawInsideGrid();
SDL_UpdateRect(screen,0,0,0,0); //rafraichir l'affichage
}




/*****************
 * dessine les chiffres dans les cases
 *****************/
void AppGame::drawInsideGrid(){
for (int x=0;x<4;x++){
	for (int y=0;y<4;y++){

		if (tabNum[x][y]>0){

			//couleur de fond dépend de la valeur du chiffre
			if (tabNum[x][y]==2){
				roundedBoxRGBA(screen,left+(ep*3)+(x*stepW),top+(ep*3)+(y*stepH),left-(ep*3)+(x*stepW)+stepW,top-(ep*3)+(y*stepH)+stepH,5,200,200,200,150);
			}
			if (tabNum[x][y]==4){
							roundedBoxRGBA(screen,left+(ep*3)+(x*stepW),top+(ep*3)+(y*stepH),left-(ep*3)+(x*stepW)+stepW,top-(ep*3)+(y*stepH)+stepH,5,150,150,150,150);
			}
			if (tabNum[x][y]==8){
							roundedBoxRGBA(screen,left+(ep*3)+(x*stepW),top+(ep*3)+(y*stepH),left-(ep*3)+(x*stepW)+stepW,top-(ep*3)+(y*stepH)+stepH,5,100,100,100,150);
			}
			if (tabNum[x][y]==16){
							roundedBoxRGBA(screen,left+(ep*3)+(x*stepW),top+(ep*3)+(y*stepH),left-(ep*3)+(x*stepW)+stepW,top-(ep*3)+(y*stepH)+stepH,5,250,0,0,150);
			}
			if (tabNum[x][y]==32){
							roundedBoxRGBA(screen,left+(ep*3)+(x*stepW),top+(ep*3)+(y*stepH),left-(ep*3)+(x*stepW)+stepW,top-(ep*3)+(y*stepH)+stepH,5,200,0,0,150);
			}
			if (tabNum[x][y]==64){
							roundedBoxRGBA(screen,left+(ep*3)+(x*stepW),top+(ep*3)+(y*stepH),left-(ep*3)+(x*stepW)+stepW,top-(ep*3)+(y*stepH)+stepH,5,100,0,0,150);
			}
			if (tabNum[x][y]==128){
							roundedBoxRGBA(screen,left+(ep*3)+(x*stepW),top+(ep*3)+(y*stepH),left-(ep*3)+(x*stepW)+stepW,top-(ep*3)+(y*stepH)+stepH,5,250,250,0,150);
			}
			if (tabNum[x][y]==256){
							roundedBoxRGBA(screen,left+(ep*3)+(x*stepW),top+(ep*3)+(y*stepH),left-(ep*3)+(x*stepW)+stepW,top-(ep*3)+(y*stepH)+stepH,5,200,200,0,150);
			}
			if (tabNum[x][y]==512){
							roundedBoxRGBA(screen,left+(ep*3)+(x*stepW),top+(ep*3)+(y*stepH),left-(ep*3)+(x*stepW)+stepW,top-(ep*3)+(y*stepH)+stepH,5,100,100,0,150);
			}
			if (tabNum[x][y]==1024){
							roundedBoxRGBA(screen,left+(ep*3)+(x*stepW),top+(ep*3)+(y*stepH),left-(ep*3)+(x*stepW)+stepW,top-(ep*3)+(y*stepH)+stepH,5,0,100,0,150);
			}
			if (tabNum[x][y]==2048){
							roundedBoxRGBA(screen,left+(ep*3)+(x*stepW),top+(ep*3)+(y*stepH),left-(ep*3)+(x*stepW)+stepW,top-(ep*3)+(y*stepH)+stepH,5,0,200,0,150);
			}
			//couleur des chiffres
			fgColor.r=255;fgColor.g=255;fgColor.b=255;

			//convertir chiffre en chaine de caracteres
			std::ostringstream ss;
			ss<< tabNum[x][y];

			//rendu, centrage et affichage du chiffre
			renderNumber=TTF_RenderText_Blended(fontMedium,ss.str().c_str(),fgColor);
			rectDst.x=left+ep+(x*stepW)+((stepW/2)-(renderNumber->w/2));
			rectDst.y=top+ep+(y*stepH)+((stepH/2)-(renderNumber->h/2));
			SDL_BlitSurface(renderNumber,NULL,screen,&rectDst);
			SDL_FreeSurface(renderNumber);
		}//if
	}//for
}//for
}




/******************
 * Dessiner la grille seule...
 ******************/
void AppGame::drawGrid(){
//bord de la grille
boxRGBA(screen,left,top,left+ep,top+height,255,255,255,200);
boxRGBA(screen,left,top,left+width,top+ep,255,255,255,200);
boxRGBA(screen,left+width-ep,top,left+width,top+height,255,255,255,200);
boxRGBA(screen,left,top+height-ep,left+width,top+height,255,255,255,200);
//barres verticales
for(int y=1;y<=nbCase;y++){
	boxRGBA(screen,left,top+(y*stepH),left+width,top+(y*stepH)+ep,255,255,255,200);
}//for
//barres horizontales
for(int x=1;x<=nbCase;x++){
	boxRGBA(screen,left+(x*stepW),top,left+(x*stepW)+ep,top+height,255,255,255,200);
}//for
}



/**********************
 * convertir la position de la souris en un evenement sur un element cliquable
 **********************/
int AppGame::mouseConvertPos(int x, int y){
// haut 1
// droite 2
// bas 3
//gauche 4
//new 5
//quit 6

if (x>20 && x<180 && y>20 && y<100){
	return 5;
}

if (x>20 && x<180 && y>110 && y<190){
	return 6;
}

if ((x>(left+(width/2)-20)) && (x<(left+(width/2)+20)) && (y>top-20) && (y<top-2)){
	return 1;
}
if ((x>(left+(width/2)-20)) && (x<(left+(width/2)+20)) && (y>top+height+5) && (y<top+height+20)){
	return 3;
}
if ((x>(left-20)) && (x<(left-2)) && (y>(top+(height/2)-20)) && (y<(top+(height/2)+20))){
	return 4;
}
if ((x>(left+width+5)) && (x<(left+width+20)) && (y>(top+(height/2)-20)) && (y<(top+(height/2)+20))){
	return 2;
}

//retourne rien sinon
return 0;
}



/********************
 * dessine les elements cliquables et modifables
 ********************/
void AppGame::drawTools(){
	//dessiner les fleches de direction
filledTrigonRGBA(screen,left+(width/2),top -20,left+(width/2)-20,top-2,left+(width/2)+20,top-2,255,255,255,200);
filledTrigonRGBA(screen,left+(width/2),top+height+20,left+(width/2)-20,top+height+5,left+(width/2)+20,top+height+5,255,255,255,200);
filledTrigonRGBA(screen,left-20,top+(height/2),left-2,top+(height/2)-20,left-2,top+(height/2)+20,255,255,255,200);
filledTrigonRGBA(screen,left+width+20,top+(height/2),left+width+5,top+(height/2)-20,left+width+5,top+(height/2)+20,255,255,255,200);

//nouveau jeu
boxRGBA(screen,20,20,180,100,200,200,0,200);

	rectDst.x=65;
	rectDst.y=40;
	renderNewGame=TTF_RenderText_Blended(fontMedium,"NEW",fgColor);
	SDL_BlitSurface(renderNewGame,NULL,screen,&rectDst);
	SDL_FreeSurface(renderNewGame);

	//quitter jeu
	boxRGBA(screen,20,110,180,190,200,0,0,200);

		rectDst.x=55;
		rectDst.y=130;
		renderQuitGame=TTF_RenderText_Blended(fontMedium,"QUIT",fgColor);
		SDL_BlitSurface(renderQuitGame,NULL,screen,&rectDst);
		SDL_FreeSurface(renderQuitGame);


//score
boxRGBA(screen,200,20,460,70,0,150,0,200);
//rendu, centrage et affichage du chiffre
//convertir chiffre en chaine de caracteres
		std::ostringstream ss;
		ss<< score;
	renderScore=TTF_RenderText_Blended(fontMedium,ss.str().c_str(),fgColor);
	rectDst.x=210;
	rectDst.y=27;
	SDL_BlitSurface(renderScore,NULL,screen,&rectDst);
	SDL_FreeSurface(renderScore);

//copyright
	rectDst.x=10;
	rectDst.y=624;
	renderCopyright=TTF_RenderText_Blended(fontVerySmall,"Tondeur Herve Copyright (c) 2014 licence GPL v3.0",fgColor);
	SDL_BlitSurface(renderCopyright,NULL,screen,&rectDst);
	SDL_FreeSurface(renderCopyright);

//dessiner le game over
	//gameOver=true;
if (gameOver){
	roundedBoxRGBA(screen,80,370,415,430,10,255,255,255,150);
	rectDst.x=90;
	rectDst.y=370;
	fgColor.r=255;fgColor.g=0;fgColor.b=0;
	renderGameOver=TTF_RenderText_Blended(fontLarge,"GAME OVER",fgColor);
	SDL_BlitSurface(renderGameOver,NULL,screen,&rectDst);
	SDL_FreeSurface(renderGameOver);
}

//dessiner le game Winer
	//gameWin=true;
if (gameWin){
	roundedBoxRGBA(screen,90,370,395,425,10,255,255,255,150);
	rectDst.x=100;
	rectDst.y=370;
	fgColor.r=0;fgColor.g=255;fgColor.b=0;
	renderGameWin=TTF_RenderText_Blended(fontLarge,"GAME WIN",fgColor);
	SDL_BlitSurface(renderGameWin,NULL,screen,&rectDst);
	SDL_FreeSurface(renderGameWin);
}


}




/******************
 * creer une nouvelle partie
 ******************/
void AppGame::newGame(){

srand(time(NULL));
gameOver=false;
gameWin=false;
score=0;

	//initialiser les tableaux
		for (int x=0;x<4;x++){
			for (int y=0;y<4;y++){
				tabNum[x][y]=0;
			}//for
		}//for

//positionner 2 valeurs au hasard soit 2 soit 4 dans 2 cases différentes
int nbOK=0;
while(nbOK<2){
	int x=rand()%4;int y=rand()%4;
	if (tabNum[x][y]==0) {tabNum[x][y]=pow(2,(rand()%2)+1); nbOK++;}
} //while
}


/**
 * liberer les ressources
 */
AppGame::~AppGame() {
	TTF_CloseFont(fontVerySmall);
	TTF_CloseFont(fontSmall);
	TTF_CloseFont(fontMedium);
	TTF_CloseFont(fontLarge);
}

} /* namespace game */
