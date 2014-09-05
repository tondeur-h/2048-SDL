/*
 * AppGame.cpp
 *
 *  Created on: Aug 22, 2014
 *      Author: herve
 */

#include "AppGame.h"
#include "appnew.h"
#include "globalVars.h"
#include "sdl_widgets.h"


namespace game {

//const mode video
const int SCREENWIDTH=480;
const int SCREENHEIGHT=640;
const int SCREENBPP=0;


//coordonn�es de la souris
int X,Y;

//coordonn�es grille
int ep=2;
int left=40;
int top=200;
int width=400;
int height=400;

//calcul des pas d'espacement des cases
int stepW;
int stepH;

//quelques variables utiles
int direction;
int resultCheck;

//pour l'affichage
TTF_Font* fontLarge;
TTF_Font* fontMedium;
TTF_Font* fontSmall;
TTF_Font* fontHalfSmall;
TTF_Font* fontVerySmall;
SDL_Color fgColor;
SDL_Rect rectDst;
SDL_Surface* renderNumber;
SDL_Surface* renderBtn;
SDL_Surface* renderGameOver;
SDL_Surface* renderGameWin;

SDL_widgets* widgets;

//fichier score
std::ifstream iFile;
std::ofstream oFile;

/*************
* constructeur
**************/
AppGame::AppGame() {
    widgets=new SDL_widgets(screen);
nbCase=4;
}

//--------DEBUG---------------
void AppGame::say(std::string txt){
std::cout<<txt<<std::endl;}
//----------------------------


/*********************************
 * Initialise SDL et le mode video
 *********************************/
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
	fontMedium=TTF_OpenFont("font.fnt",38);
	fontSmall=TTF_OpenFont("font.fnt",26);
    fontHalfSmall=TTF_OpenFont("font.fnt",21);
	fontVerySmall=TTF_OpenFont("font.fnt",14);

    //lire le meilleur score
    iFile.open("score.scr",std::ios::in);
    if (iFile.is_open()){iFile>>bestScore;iFile.close();}

	return true;
}



/**************************
 * Boucle principale du jeu
 **************************/
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
							//if (direction==8) {newChallenge();direction=0;}
							if (direction==6) {GoOut=true;direction=0;}
							if (direction==7) {
                                    if (!__LINUX__){system("start help.html");}
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




/**************************************
* check Best Score stocker si sup�rieur
***************************************/
void AppGame::checkBestScore(){
if (score>bestScore){
        bestScore=score;
    oFile.open("score.scr",std::ios::out);
    if (oFile.is_open()){
        oFile<<bestScore;
        oFile.close();
    } //if
} //if
}




/*******************************************
 * Verifier si le tableau n'est pas complet.
 *******************************************/
int AppGame::checkTab(){
bool compactable=false;
bool OK2048=false;
int nbVide=0;
/* valeurs de retour
 * 0 fin de jeu
 * 1 nouveau nombre
 * 2 plus de case mais compactable
 * 3 GOAL atteint winner
 */

//---------------------------------------------------------------------
//tester si 2048 atteint => the winner
//prioritaire au autres actions
for (int x=0;x<nbCase;x++){
	for (int y=0;y<nbCase;y++){
		if (tabNum[x][y]==GOAL) {OK2048=true;break;}
	}
}
//si 2048 atteint fin du jeu...
if (OK2048) return 3;


//---------------------------------------------------------------------
//calculer nombre de caseVide
for (int x=0;x<nbCase;x++){
	for (int y=0;y<nbCase;y++){
		if (tabNum[x][y]==0) {nbVide++;}
	}//for
}//for

if (nbVide>0){
//ajouter un nombre entre 2 et 4
	int nbOK=0;
	while(nbOK<1){
		int x=rand()%nbCase;int y=rand()%nbCase;
		if (tabNum[x][y]==0) {tabNum[x][y]=pow(2,(rand()%2)+1); nbOK++;}
	} //while
}

//si nbVide>1
if (nbVide>1){ return 1;}

//---------------------------------------------------------------------
//Tester si compactable doit y avoir 2 valeurs identique contigue
//tester les lignes
for (int y=0;y<nbCase;y++){
	for (int x=0;x<nbCase-1;x++){
		if (tabNum[x+1][y]==tabNum[x][y]) {compactable=true;break;}
	}//for
}//for

//tester les colonnes
for (int x=0;x<nbCase;x++){
	for (int y=0;y<nbCase-1;y++){
		if (tabNum[x][y+1]==tabNum[x][y]) {compactable=true;break;}
	}//for
}//for

//si OK  sortir on peut continuer
if (compactable) {return 2;}

//---------------------------------------------------------------------
//fin de jeu, plus aucun coup possible
return 0;
}




/****************************************************
 * calcule les sommes du tableau apret un deplacement
 ****************************************************/
void AppGame::calculateTab(int dir){
int z=0;

//*****************direction haut**************************
if (dir==1){
	//compactages des nombres
	for (int x=0;x<nbCase;x++){
		for (int y=1;y<nbCase;y++){
                //si l'element n'est pas vide
			if (tabNum[x][y]>0){z=y;}
        //pour chaque case de gauche possible
			while(z-1>=0){
                    //si element de gauche est vide=> deplacer l'element vers la gauche
				if (tabNum[x][z-1]==0){tabNum[x][z-1]=tabNum[x][z];tabNum[x][z]=0;}
				z--;
			}//while
		}//for y
	}//for x


	//calcul des sommes
	for (int x=0;x<nbCase;x++){
			for (int y=0;y<nbCase;y++){
				if (tabNum[x][y+1]==tabNum[x][y]){
								tabNum[x][y]=tabNum[x][y]*2;score=score+tabNum[x][y];tabNum[x][y+1]=0;
							}
			}//for y
		}//for x


//second d�placement
for (int x=0;x<nbCase;x++){
//pour x=1 a x<4
			//si (x)>0 alors z=x //fin si
			//tant que z-1>0
				//si (z-1)=0 alors (z-1)=(z) et (z)=(0)
				//z--
			//fin tant que
		//fin pour

	for (int y=1;y<nbCase;y++){
		if (tabNum[x][y]>0){z=y;}
		while(z-1>=0){
			if (tabNum[x][z-1]==0){tabNum[x][z-1]=tabNum[x][z];tabNum[x][z]=0;}
			z--;
		}//while
	}//for y
}//for x
}//if

//*****************direction droite**************************
if (dir==2){
	//compactage des nombres
		for (int y=0;y<nbCase;y++){
			for (int x=nbCase-2;x>=0;x--){
				if (tabNum[x][y]>0){z=x;}
				while(z+1<=nbCase-1){
					if (tabNum[z+1][y]==0){tabNum[z+1][y]=tabNum[z][y];tabNum[z][y]=0;}
					z++;
				}//while
			}//for x
		}//for y

	//calculer les sommes
		for (int y=0;y<nbCase;y++){
			for (int x=0;x<nbCase-1;x++){
				if (tabNum[x+1][y]==tabNum[x][y]){
					tabNum[x][y]=tabNum[x][y]*2;score=score+tabNum[x][y];tabNum[x+1][y]=0;
				}
			}//for x
		}//for y

	//deplacer les elelements
	for (int y=0;y<nbCase;y++){
		for (int x=nbCase-2;x>=0;x--){
			if (tabNum[x][y]>0){z=x;}
			while(z+1<=nbCase-1){
				if (tabNum[z+1][y]==0){tabNum[z+1][y]=tabNum[z][y];tabNum[z][y]=0;}
				z++;
			}//while
		}//for x
	}//for y
	}//if


//*****************direction bas**************************
if (dir==3){

	//compactage des nombres
	//second deplacement
		for (int x=0;x<nbCase;x++){
				for (int y=nbCase-2;y>=0;y--){
					if (tabNum[x][y]>0){z=y;}
					while(z+1<=nbCase-1){
						if (tabNum[x][z+1]==0){tabNum[x][z+1]=tabNum[x][z];tabNum[x][z]=0;}
						z++;
					}//while
				}//for y
			}//for x

	//calcul des sommes
		for (int x=0;x<nbCase;x++){
				for (int y=nbCase-1;y>=1;y--){
					if (tabNum[x][y-1]==tabNum[x][y]){
									tabNum[x][y]=tabNum[x][y]*2;score=score+tabNum[x][y];tabNum[x][y-1]=0;
								}
				}//for y
			}//for x

	//second deplacement
	for (int x=0;x<nbCase;x++){
			for (int y=nbCase-2;y>=0;y--){
				if (tabNum[x][y]>0){z=y;}
				while(z+1<=nbCase-1){
					if (tabNum[x][z+1]==0){tabNum[x][z+1]=tabNum[x][z];tabNum[x][z]=0;}
					z++;
				}//while
			}//for y
		}//for x
}

//*****************direction gauche**************************
if (dir==4){
	//compactage des valeurs
	for (int y=0;y<nbCase;y++){
		for (int x=1;x<nbCase;x++){
			if (tabNum[x][y]>0){z=x;}
			while(z-1>=0){
				if (tabNum[z-1][y]==0){tabNum[z-1][y]=tabNum[z][y];tabNum[z][y]=0;}
				z--;
			}//while
		}//for x
	}//for y

//calculer les sommes
	for (int y=0;y<nbCase;y++){
		for (int x=nbCase-1;x>=1;x--){
			if (tabNum[x-1][y]==tabNum[x][y]){
				tabNum[x][y]=tabNum[x][y]*2;score=score+tabNum[x][y];tabNum[x-1][y]=0;
			}
		}//for x
	}//for y

//second d�placement
for (int y=0;y<nbCase;y++){
//pour x=1 a x<4
			//si (x)>0 alors z=x //fin si
			//tant que z-1>0
				//si (z-1)=0 alors (z-1)=(z) et (z)=(0)
				//z--
			//fin tant que
		//fin pour

	for (int x=1;x<nbCase;x++){
		if (tabNum[x][y]>0){z=x;}
		while(z-1>=0){
			if (tabNum[z-1][y]==0){tabNum[z-1][y]=tabNum[z][y];tabNum[z][y]=0;}
			z--;
		}//while
	}//for x
}//for y
}//if

}




/***********************************************
 * dessine le tableau et les valeurs du tableau
 ***********************************************/
void AppGame::drawTab(){
//vider l'�cran
boxRGBA(screen,0,0,SCREENWIDTH,SCREENHEIGHT,0,0,0,255);
drawGrid();
drawTools();
drawInsideGrid();
SDL_UpdateRect(screen,0,0,0,0); //redessiner l'�cran
}




/*************************************
 * dessine les chiffres dans les cases
 *************************************/
void AppGame::drawInsideGrid(){
int r,g,b,a;
for (int x=0;x<nbCase;x++){
	for (int y=0;y<nbCase;y++){

		if (tabNum[x][y]>0){

			//couleur de fond d�pend de la valeur du chiffre
			if (tabNum[x][y]==2){r=200;g=200;b=200;a=150;}
			if (tabNum[x][y]==4){r=150;g=150;b=150;a=150;}
			if (tabNum[x][y]==8){r=100;g=100;b=100;a=150;}
			if (tabNum[x][y]==16){r=250;g=0;b=0;a=150;}
			if (tabNum[x][y]==32){r=200;g=0;b=0;a=150;}
 			if (tabNum[x][y]==64){r=100;g=0;b=0;a=150;}
            if (tabNum[x][y]==128){r=250;g=250;b=0;a=150;}
            if (tabNum[x][y]==256){r=200;g=200;b=0;a=150;}
			if (tabNum[x][y]==512){r=100;g=100;b=0;a=150;}
			if (tabNum[x][y]==1024){r=0;g=100;b=0;a=150;}
			if (tabNum[x][y]==2048){r=0;g=200;b=0;a=150;}

            //dessiner la box des chiffres
            roundedBoxRGBA(screen,left+(ep*3)+(x*stepW),top+(ep*3)+(y*stepH),left-(ep*3)+(x*stepW)+stepW,top-(ep*3)+(y*stepH)+stepH,3,r,g,b,a);

			//couleur des chiffres
			fgColor.r=255;fgColor.g=255;fgColor.b=255;

			//convertir chiffre en chaine de caracteres
			std::ostringstream ss;ss<< tabNum[x][y];

			//rendu, centrage et affichage du chiffre
			if (nbCase<5){
			renderNumber=TTF_RenderText_Blended(fontMedium,ss.str().c_str(),fgColor);
			}

			if (nbCase>=5 && nbCase<=6){
                renderNumber=TTF_RenderText_Blended(fontSmall,ss.str().c_str(),fgColor);
			}

			if (nbCase>=7 && nbCase<=8){
			renderNumber=TTF_RenderText_Blended(fontHalfSmall,ss.str().c_str(),fgColor);
			}

			if (nbCase>8){
			renderNumber=TTF_RenderText_Blended(fontVerySmall,ss.str().c_str(),fgColor);
			}

			rectDst.x=left+ep+(x*stepW)+((stepW/2)-(renderNumber->w/2));
			rectDst.y=top+ep+(y*stepH)+((stepH/2)-(renderNumber->h/2));
			SDL_BlitSurface(renderNumber,NULL,screen,&rectDst);
			SDL_FreeSurface(renderNumber);
		}//if
	}//for
}//for
}




/*****************************
 * Dessiner la grille seule...
 *****************************/
void AppGame::drawGrid(){
//cadre de la grille
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



/******************************************
 * convertir la position de la souris
 * en un evenement sur un element cliquable
 ******************************************/
int AppGame::mouseConvertPos(int x, int y){
// haut 1
// droite 2
// bas 3
// gauche 4
// new 5
// quit 6
// help 7
// Challenge 8

//click bouton new
if (x>20 && x<180 && y>50 && y<90){
	return 5;
}//if

//click bouton Challenge
//if (x>20 && x<180 && y>70 && y<110){
//	return 8;
//}//if


//click bouton quit
if (x>20 && x<180 && y>120 && y<190){
	return 6;
}//if

//click bouton help
if (x>400 && x<460 && y>0 && y<20){
	return 7;
}//if


//sortir si game over n'autorise dans ce cas que le bouton new & quit
if (gameOver || gameWin) {return 0;}

//click fleche haut
if ((x>(left+(width/2)-20)) && (x<(left+(width/2)+20)) && (y>top-20) && (y<top-2)){
	hit++;return 1;
}//if

//click fleche bas
if ((x>(left+(width/2)-20)) && (x<(left+(width/2)+20)) && (y>top+height+5) && (y<top+height+20)){
	hit++;return 3;
}//if

//click fleche gauche
if ((x>(left-20)) && (x<(left-2)) && (y>(top+(height/2)-20)) && (y<(top+(height/2)+20))){
	hit++;return 4;
}//if

//click fleche droite
if ((x>(left+width+5)) && (x<(left+width+20)) && (y>(top+(height/2)-20)) && (y<(top+(height/2)+20))){
	hit++;return 2;
}//if

//retourne rien
return 0;
}


/*********************************************
* permet de dessiner les boutons et les labels
**********************************************/
void AppGame::drawBtn(int x1,int y1,int w,int h,std::string txt,TTF_Font* font, int bgr, int bgg, int bgb, int bga, SDL_Color fgC, bool shadow, bool round, bool center){

//arrondis des boites
int rn=5;
if (round){rn=5;} else {rn=1;}

//ombre de la boite
if (shadow){roundedBoxRGBA(screen,x1,y1,x1+w,y1+h,rn,200,200,200,80);}

//corps de la boite
roundedBoxRGBA(screen,x1+2,y1+2,x1+w,y1+h,rn,bgr,bgg,bgb,bga);
	renderBtn=TTF_RenderText_Blended(font,txt.c_str(),fgC);
    //centrage du texte
    if (center){
    rectDst.x=((w/2)-(renderBtn->w/2))+x1;
	rectDst.y=((h/2)-(renderBtn->h/2))+y1;
    } else
    {
    rectDst.x=x1;
	rectDst.y=y1;
    }//if
	SDL_BlitSurface(renderBtn,NULL,screen,&rectDst);
	SDL_FreeSurface(renderBtn);
}


/***********************************************
 * dessine les elements cliquables et modifables
 ***********************************************/
void AppGame::drawTools(){
	//dessiner les fleches de directions
filledTrigonRGBA(screen,left+(width/2),top -20,left+(width/2)-20,top-2,left+(width/2)+20,top-2,255,255,255,200);
filledTrigonRGBA(screen,left+(width/2),top+height+20,left+(width/2)-20,top+height+5,left+(width/2)+20,top+height+5,255,255,255,200);
filledTrigonRGBA(screen,left-20,top+(height/2),left-2,top+(height/2)-20,left-2,top+(height/2)+20,255,255,255,200);
filledTrigonRGBA(screen,left+width+20,top+(height/2),left+width+5,top+(height/2)-20,left+width+5,top+(height/2)+20,255,255,255,200);

//couleur du texte
fgColor.r=255; fgColor.g=255;fgColor.b=255;

//nombre de click

//drawBtn(2,2,100,20,"Click",fontVerySmall,0,0,0,0,fgColor,false,false,false);
//convertir chiffre en chaine de caracteres
    std::ostringstream shs;shs<< hit;
//widgets->drawLabel(2,2,shs.str().c_str(),fontVerySmall,fgColor);

drawBtn(2,2,200,20,shs.str().c_str(),fontVerySmall,0,0,0,0,fgColor,false,false,false);

//nouveau jeu
drawBtn(20,50,160,40,"NEW",fontMedium,200,200,0,200,fgColor,true,true,true);

//nouveau challenge
//drawBtn(20,70,160,40,"Challenge",fontSmall,159,121,238,200,fgColor,true,true,true);

//quitter jeu
drawBtn(20,120,160,70,"QUIT",fontMedium,200,0,0,200,fgColor,true,true,true);

//bouton help
drawBtn(400,0,60,20,"Help",fontVerySmall,255,140,0,200,fgColor,true,true,true);

//score text
drawBtn(200,20,100,20,"Score",fontSmall,0,0,0,0,fgColor,false,false,false);
//convertir chiffre en chaine de caracteres
    std::ostringstream ss;ss<< score;
    drawBtn(200,50,260,40,ss.str().c_str(),fontMedium,0,150,0,200,fgColor,false,false,false);

//best score
drawBtn(200,100,200,20,"Best Score",fontSmall,0,0,0,0,fgColor,false,false,false);
//convertir chiffre en chaine de caracteres
    std::ostringstream sbs;sbs<< bestScore;
    drawBtn(200,130,260,40,sbs.str().c_str(),fontMedium,0,0,150,200,fgColor,false,false,false);


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
} //if

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
} //if
}




/***************************
 * creer une nouvelle partie
 ***************************/
void AppGame::newGame(){
//init les valeurs par defaut
srand(time(NULL));
gameOver=false;
gameWin=false;
score=0;
hit=0;

AppNew* nouv=NULL;
nouv=new AppNew();
delete (nouv);

//calcul des pas d'espacement des cases
stepW=width/nbCase;
stepH=height/nbCase;

if (boolChal){
    newChallenge();
    return;
}
	//initialiser le tableau avec des zero
		for (int x=0;x<nbCase;x++){
			for (int y=0;y<nbCase;y++){
				tabNum[x][y]=0;
			}//for
		}//for

//positionner 2 valeurs au hasard soit 2 soit 4 dans 2 cases diff�rentes
int nbOK=0;
while(nbOK<2){
	int x=rand()%nbCase;int y=rand()%nbCase;
	if (tabNum[x][y]==0) {tabNum[x][y]=pow(2,(rand()%2)+1); nbOK++;}
	//if (tabNum[x][y]==0) {tabNum[x][y]=pow(2,(rand()%9)+1); nbOK++;}
} //while
}



/****************************
 * creer un nouveau Challenge
 * partie du jeu avanc�e
 ****************************/
void AppGame::newChallenge(){
//init les valeurs par d�faut
srand(time(NULL));
gameOver=false;
gameWin=false;
score=0;
hit=0;

	//initialiser le tableau avec des zero
		for (int x=0;x<nbCase;x++){
			for (int y=0;y<nbCase;y++){
				tabNum[x][y]=0;
			}//for
		}//for

//positionner maxcase<=(nbCase/4)*12 valeurs au hasard soit 2,4,8,16,32,64,128,256,512 dans maxcase cases diff�rentes
int nbOK=0;
int maxPower=9;
int maxCase=(rand()%(((nbCase/4)*12)+1)+2);
while(nbOK<maxCase){
	int x=rand()%nbCase;int y=rand()%nbCase;
	if (tabNum[x][y]==0) {tabNum[x][y]=pow(2,(rand()%maxPower)+1); nbOK++;}
} //while
}




/***********************************
 * liberer les ressources des fontes
 ***********************************/
AppGame::~AppGame() {
	TTF_CloseFont(fontVerySmall);
    TTF_CloseFont(fontHalfSmall);
	TTF_CloseFont(fontSmall);
	TTF_CloseFont(fontMedium);
	TTF_CloseFont(fontLarge);
    free(widgets);
}

} /* namespace game */
