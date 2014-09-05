#include "appnew.h"

#include "sdl_widgets.h"

SDL_Color fgColor={255,255,255,255};
SDL_Event eventNew;

SDL_Surface* h1;
SDL_Surface* h2;
SDL_Surface* h3;
SDL_Surface* h4;

TTF_Font* fontMed=NULL;
TTF_Font* fontSma=NULL;
TTF_Font* fontVerySma=NULL;

SDL_widgets* widgets;

bool GoOutNew=false;
int mX,mY;

int numImg=1;




/*******************************
* Constructeur definit les
* elements utile dans cette page
********************************/
AppNew::AppNew()
{

widgets=new SDL_widgets(screen);

//init les valeurs par defaut
GoOutNew=false;
srand(time(NULL));
gameOver=false;
gameWin=false;
score=0;

fontMed=TTF_OpenFont("font.fnt",38);
fontSma=TTF_OpenFont("font.fnt",22);
fontVerySma=TTF_OpenFont("font.fnt",10);

IMG_Init(IMG_INIT_PNG);

h1=IMG_Load("h1.png");
h2=IMG_Load("h2.png");
h3=IMG_Load("h3.png");
h4=IMG_Load("h4.png");

loopNew();
}



/*********************************
* Dessine l'écran des options...
**********************************/
void AppNew::drawScreen(){
//vider l'écran
boxRGBA(screen,0,0,480,640,0,0,0,255);
//label nb cases
 std::ostringstream sgs;sgs<<GOAL;
widgets->drawLabel(50,10,std::string("Grid size ?      Goal : ")+sgs.str(), fontSma,fgColor);
//slider
widgets->drawSlider(50,40,380,30,fontSma,200,200,0,200,fgColor,3,12,nbCase,true);
//checkbox
widgets->drawCheckBox(50,100,fontSma,"Shuffled Game...",230,230,0,200,fgColor,boolChal);
//btnOK
widgets->drawBtn(50,140,380,60,"GO!",fontMed,200,200,0,200,fgColor,true,true,true);
lineRGBA(screen,10,210,470,210,255,255,255,255);
//reset bestScore
if (bestScore>0){
widgets->drawLabel(50,230,"Reset Best Score ?",fontSma,fgColor);
widgets->drawBtn(300,218,100,50,"YES",fontMed,255,0,0,255,fgColor,true,true,true);
}
lineRGBA(screen,10,275,470,275,255,255,255,255);
//slider image aide
widgets->drawSlider(50,285,380,15,fontVerySma,0,200,0,200,fgColor,1,4,numImg,false);
widgets->drawLabel(220,300,"HELP",fontSma,fgColor);
if (numImg==1) widgets->drawImage(92,335,h1);
if (numImg==2) widgets->drawImage(92,335,h2);
if (numImg==3) widgets->drawImage(92,335,h3);
if (numImg==4) widgets->drawImage(92,335,h4);

SDL_UpdateRect(screen,0,0,0,0); //redessiner l'écran
}




/*********************************************************
* affecte le but du jeu en fonction du niveau de la grille
**********************************************************/
void AppNew::setGOAL(int nbc){
if (nbc<=5) {GOAL=2048;}
if (nbc>5 && nbc<=8) {GOAL=4096;}
if (nbc>8 && nbc<=11) {GOAL=8192;}
if (nbc>11) {GOAL=16384;}
}




/*************************
* Boucle du jeu
**************************/
void AppNew::loopNew(){
//interdire les evenements clavier
SDL_EventState(SDL_KEYDOWN,SDL_IGNORE);
SDL_EventState(SDL_KEYUP,SDL_IGNORE);

//dessiner le premier ecran
drawScreen();

	while (!GoOutNew){

		if (SDL_WaitEvent(&eventNew)==1){
		//controle clavier et souris
		switch(eventNew.type){


		case SDL_MOUSEBUTTONDOWN:
						if (eventNew.button.button==SDL_BUTTON_LEFT){
							mX=eventNew.button.x;mY=eventNew.button.y;
							if (widgets->mouseInBox(mX,mY,50,140,380,60,1)==1) {GoOutNew=true;}
                            if (widgets->mouseInBox(mX,mY,50,40,20,30,2)==2) {nbCase--;if (nbCase<3) {nbCase=3;};setGOAL(nbCase);}
                            if (widgets->mouseInBox(mX,mY,50,285,20,15,6)==6) {numImg--;if (numImg<1) {numImg=1;}}
                            if (widgets->mouseInBox(mX,mY,300,218,100,50,5)==5) {resetBestScore();}
                            if (widgets->mouseInBox(mX,mY,390,40,20,30,3)==3) {nbCase++;if (nbCase>12) {nbCase=12;};setGOAL(nbCase);}
                            if (widgets->mouseInBox(mX,mY,390,285,20,15,7)==7) {numImg++;if (numImg>4) {numImg=4;}}
                            if (widgets->mouseInBox(mX,mY,50,100,30,30,4)==4) {if (boolChal==true) {boolChal=false;} else {boolChal=true;};}
						}//if
						break;

		} //switch

			//dessine le jeu
			drawScreen();

	} //if

	}//while
}




/******************
* reset Best Score
*******************/
void AppNew::resetBestScore(){
    std::ofstream oFile;
        bestScore=0;
    oFile.open("score.scr",std::ios::out);
    if (oFile.is_open()){
        oFile<<bestScore;
        oFile.close();
    } //if
}



/******************************
* liberer les ressources
*******************************/
AppNew::~AppNew()
{
    TTF_CloseFont(fontMed);
    TTF_CloseFont(fontSma);
    TTF_CloseFont(fontVerySma);

    SDL_FreeSurface(h1);
    SDL_FreeSurface(h2);
    SDL_FreeSurface(h3);
    SDL_FreeSurface(h4);

    free(widgets);
}
