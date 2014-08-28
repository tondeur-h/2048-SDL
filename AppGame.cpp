/*
 * AppGame.cpp
 *
 *  Created on: 22 Aout 2014
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


#include "AppGame.h"

namespace game {

//const mode video
const int SCREENWIDTH=480;
const int SCREENHEIGHT=640;
const int SCREENBPP=0;

//tick pour le calcul du frame rate
long int firstTick, lastTick;

//coordonn�es de la souris
int X,Y;

//coordonn�es grille
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
int resultCheck;
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
SDL_Surface* renderBtn;
SDL_Surface* renderGameOver;
SDL_Surface* renderGameWin;

//fichier score
std::ifstream iFile;
std::ofstream oFile;
long int bestScore;

AppGame::AppGame() {
	newGame();
}


void debug(std::string deb){
	std::cout<<deb<<std::endl;
}


/**
 * Initialise SDL et le mode video
 */
bool AppGame::initSDL(){
	if (SDL_Init(SDL_INIT_EVERYTHING)==-1){
		std::cout<<"Erreur init SDL!"<<std::endl;
		return false;
	}//if

	if ((screen=SDL_SetVideoMode(SCREENWIDTH,SCREENHEIGHT,SCREENBPP,SDL_SWSURFACE))==NULL){
		std::cout<<"Erreur set Video SDL!"<<std::endl;
				return false;
	}//if

	SDL_WM_SetCaption("2p11 Game version 1.0","2p11.ico");

	if (TTF_Init()==-1){
		std::cout<<"Erreur loading font!"<<std::endl;
			return false;
	}//if
	fontLarge=TTF_OpenFont("font.fnt",58);
	if (fontLarge==NULL){
		std::cout<<"Erreur loading fontLarge!"<<std::endl;
					return false;
	}

	fontMedium=TTF_OpenFont("font.fnt",38);
	if (fontMedium==NULL){
			std::cout<<"Erreur loading fontMedium!"<<std::endl;
						return false;
		}
	fontSmall=TTF_OpenFont("font.fnt",28);
	if (fontSmall==NULL){
			std::cout<<"Erreur loading fontSmall!"<<std::endl;
						return false;
		}
	fontVerySmall=TTF_OpenFont("font.fnt",14);
	if (fontVerySmall==NULL){
			std::cout<<"Erreur loading fontVerySmall!"<<std::endl;
						return false;
		}

    //lire le meilleur score
    iFile.open("score.scr",std::ios::in);
    if (iFile.is_open()){iFile>>bestScore;}


	return true;
}



/**
 * Boucle principale du jeu
 */
void AppGame::loopGame(){

//interdire les evenements clavier
SDL_EventState(SDL_KEYDOWN,SDL_IGNORE);
SDL_EventState(SDL_KEYUP,SDL_IGNORE);

//dessiner le premier ecran
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
							if (direction==5) {newGame();direction=0;}
							if (direction==8) {newChallenge();direction=0;}
							if (direction==6) {GoOut=true;direction=0;}
							if (direction==7) {
								if (__LINUX__==1){system("xdg-open 'help.html' 2>/dev/null &");} else{system("start help.html");}
                                    direction=0;
                            }
							if (direction>=1 && direction<=4)
							{
																calculateTab(direction);
																resultCheck=checkTab();
																direction=0;
																if (resultCheck==0){checkBestScore();gameOver=true;}
																if (resultCheck==3){checkBestScore();gameWin=true;}
							}

						}
						break;

		} //switch

			//dessine le jeu
			drawTab();

	} //if

	}//while
}




/*******
* check Best Score stocker si sup�rieur
********/
void AppGame::checkBestScore(){
if (score>bestScore){
        bestScore=score;
    oFile.open("score.scr",std::ios::out);
    if (oFile.is_open()){
        oFile<<bestScore;
    } //if
} //if
}




/**
 * Verifier si le tableau n'est pas complet.
 */
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
//ajouter un nombre entre 2 et 4
	int nbOK=0;
	while(nbOK<1){
		int x=rand()%4;int y=rand()%4;
		if (tabNum[x][y]==0) {tabNum[x][y]=pow(2,(rand()%2)+1); nbOK++;}
	} //while
return 1;
}
else
{
	//ajouter un nombre entre 2 et 4
		int nbOK=0;
		while(nbOK<1){
			int x=rand()%4;int y=rand()%4;
			if (tabNum[x][y]==0) {tabNum[x][y]=pow(2,(rand()%2)+1); nbOK++;}
		} //while
}

//---------------------------------------------------------------------
//Tester si compactable doit y avoir 2 valeurs identique contigue
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
//fin de jeu, plus aucun coup possible
return 0;
}




/**
 * calcule les sommes du tableau apret un deplacement
 */
void AppGame::calculateTab(int dir){
int z=0;

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


//second d�placement
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

//second d�placement
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




/**
 * dessine le tableau et les valeurs du tableau
 */
void AppGame::drawTab(){
//vider l'�cran
boxRGBA(screen,0,0,SCREENWIDTH,SCREENHEIGHT,0,0,0,255);
drawGrid();
drawTools();
drawInsideGrid();
SDL_UpdateRect(screen,0,0,0,0); //redessiner l'�cran
}




/**
 * dessine les chiffres dans les cases
 */
void AppGame::drawInsideGrid(){
for (int x=0;x<4;x++){
	for (int y=0;y<4;y++){

		if (tabNum[x][y]>0){

			//couleur de fond d�pend de la valeur du chiffre
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



void AppGame::drawBtn(int x1,int y1,int w,int h,std::string txt,TTF_Font* font, int bgr, int bgg, int bgb, int bga, SDL_Color fgC, bool shadow, bool round, bool center){

int rn=5;
if (round){rn=5;} else {rn=1;}

//ombre
if (shadow){roundedBoxRGBA(screen,x1,y1,x1+w,y1+h,rn,100,100,100,80);}
//btn
roundedBoxRGBA(screen,x1+3,y1+3,x1+w,y1+h,rn,bgr,bgg,bgb,bga);
	renderBtn=TTF_RenderText_Blended(font,txt.c_str(),fgC);
    if (center){
    rectDst.x=((w/2)-(renderBtn->w/2))+x1;
	rectDst.y=((h/2)-(renderBtn->h/2))+y1;
    } else
    {
    rectDst.x=x1;
	rectDst.y=y1;
    }
	SDL_BlitSurface(renderBtn,NULL,screen,&rectDst);
	SDL_FreeSurface(renderBtn);
}



/**
 * Dessiner la grille seule...
 */
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



/**
 * convertir la position de la souris en un evenement sur un element cliquable
 */
int AppGame::mouseConvertPos(int x, int y){
// haut 1
// droite 2
// bas 3
//gauche 4
//new 5
//quit 6
//help 7
// Chalange 8

//click bouton new
if (x>20 && x<180 && y>20 && y<60){
	return 5;	std::cout<<__LINE__<<std::endl;
}

//click bouton Challenge
if (x>20 && x<180 && y>70 && y<110){
	return 8;
}


//click bouton quit
if (x>20 && x<180 && y>120 && y<190){
	return 6;
}

//click bouton help
if (x>400 && x<460 && y>0 && y<20){
	return 7;
}


//sortir si game over n'autorise dans ce cas que le bouton new & quit
if (gameOver || gameWin) return 0;

//click fleche haut
if ((x>(left+(width/2)-20)) && (x<(left+(width/2)+20)) && (y>top-20) && (y<top-2)){
	return 1;
}

//click fleche bas
if ((x>(left+(width/2)-20)) && (x<(left+(width/2)+20)) && (y>top+height+5) && (y<top+height+20)){
	return 3;
}

//click fleche gauche
if ((x>(left-20)) && (x<(left-2)) && (y>(top+(height/2)-20)) && (y<(top+(height/2)+20))){
	return 4;
}

//click fleche droite
if ((x>(left+width+5)) && (x<(left+width+20)) && (y>(top+(height/2)-20)) && (y<(top+(height/2)+20))){
	return 2;
}

//retourne rien
return 0;
}



/**
 * dessine les elements cliquables et modifables
 */
void AppGame::drawTools(){
	//dessiner les fleches de direction
filledTrigonRGBA(screen,left+(width/2),top -20,left+(width/2)-20,top-2,left+(width/2)+20,top-2,255,255,255,200);
filledTrigonRGBA(screen,left+(width/2),top+height+20,left+(width/2)-20,top+height+5,left+(width/2)+20,top+height+5,255,255,255,200);
filledTrigonRGBA(screen,left-20,top+(height/2),left-2,top+(height/2)-20,left-2,top+(height/2)+20,255,255,255,200);
filledTrigonRGBA(screen,left+width+20,top+(height/2),left+width+5,top+(height/2)-20,left+width+5,top+(height/2)+20,255,255,255,200);

fgColor.r=255; fgColor.g=255;fgColor.b=255;

//nouveau jeu
drawBtn(20,20,160,40,"NEW",fontMedium,200,200,0,200,fgColor,true,true,true);

//nouveau challenge
drawBtn(20,70,160,40,"Challenge",fontSmall,159,121,238,200,fgColor,true,true,true);

	//quitter jeu
drawBtn(20,120,160,70,"QUIT",fontMedium,200,0,0,200,fgColor,true,false,true);

    //bouton help
drawBtn(400,0,60,20,"Help",fontVerySmall,255,140,0,200,fgColor,true,true,true);

//score text
drawBtn(200,20,100,20,"Score",fontSmall,0,0,0,0,fgColor,false,false,false);
//convertir chiffre en chaine de caracteres
    std::ostringstream ss;ss<< score;
    drawBtn(200,50,260,35,ss.str().c_str(),fontMedium,0,150,0,200,fgColor,false,false,false);

//best score
drawBtn(200,100,200,20,"Best Score",fontSmall,0,0,0,0,fgColor,false,false,false);
//convertir chiffre en chaine de caracteres
std::ostringstream sbs;sbs<< bestScore;
drawBtn(200,130,260,35,sbs.str().c_str(),fontMedium,0,0,150,200,fgColor,false,false,false);


//copyright
drawBtn(10,624,0,0,"Tondeur Herve Copyright (c) 2014 licence GPL v3.0",fontVerySmall,0,0,0,0,fgColor,false,false,false);


//dessiner "game over"
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

//dessiner "game Winer"
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




/**
 * creer une nouvelle partie
 */
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

//positionner 2 valeurs au hasard soit 2 soit 4 dans 2 cases diff�rentes
int nbOK=0;
while(nbOK<2){
	int x=rand()%4;int y=rand()%4;
	if (tabNum[x][y]==0) {tabNum[x][y]=pow(2,(rand()%2)+1); nbOK++;}
	//if (tabNum[x][y]==0) {tabNum[x][y]=pow(2,(rand()%9)+1); nbOK++;}
} //while
//demo
/*tabNum[0][0]=8;
tabNum[1][0]=4;
tabNum[2][0]=4;

tabNum[0][2]=4;
tabNum[1][2]=2;
tabNum[2][2]=2;*/

}



/**
 * creer une nouvelle partie
 */
void AppGame::newChallenge(){

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

//positionner 2 valeurs au hasard soit 2 soit 4 dans 2 cases diff�rentes
int nbOK=0;
int maxCase=(rand()%10)+2;
while(nbOK<maxCase){
	int x=rand()%4;int y=rand()%4;
	if (tabNum[x][y]==0) {tabNum[x][y]=pow(2,(rand()%9)+1); nbOK++;}
} //while
}




/**
 * liberer les ressources des fontes
 */
AppGame::~AppGame() {
	TTF_CloseFont(fontVerySmall);
	TTF_CloseFont(fontSmall);
	TTF_CloseFont(fontMedium);
	TTF_CloseFont(fontLarge);
}

} /* namespace game */
