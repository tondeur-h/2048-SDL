#include "sdl_widgets.h"


SDL_widgets::SDL_widgets(SDL_Surface* TheScreen)
{
ecran=TheScreen;
}

/******************************************
 * convertir la position de la souris
 * en un evenement sur un element cliquable
 ******************************************/
int SDL_widgets::mouseInBox(int mx, int my, int x1,int y1, int w, int h, int retour){
if (mx>x1 && mx<(x1+w) && my>y1 && my<(y1+h)){
	return retour;
}//ifecran
//retourne rien
return 0;
}

/********************
* dessine une image
********************/
void SDL_widgets::drawImage(int x1,int y1,SDL_Surface* img){
    rectDst.x=x1;
	rectDst.y=y1;
	SDL_BlitSurface(img,NULL,ecran,&rectDst);
}


/*********************************************
* permet de dessiner un bouton et son label
**********************************************/
void SDL_widgets::drawBtn(int x1,int y1,int w,int h,std::string txt,TTF_Font* font, int bgr, int bgg, int bgb, int bga, SDL_Color fgC, bool shadow, bool round, bool center){

//arrondis des boites
int rn=5;
if (round){rn=5;} else {rn=1;}

//ombre de la boite
if (shadow){roundedBoxRGBA(ecran,x1,y1,x1+w,y1+h,rn,200,200,200,80);}

//corps de la boite
roundedBoxRGBA(ecran,x1+2,y1+2,x1+w,y1+h,rn,bgr,bgg,bgb,bga);
	renderTxt=TTF_RenderText_Blended(font,txt.c_str(),fgC);
    //centrage du texte
    if (center){
    rectDst.x=((w/2)-(renderTxt->w/2))+x1;
	rectDst.y=((h/2)-(renderTxt->h/2))+y1;
    } else
    {
    rectDst.x=x1;
	rectDst.y=y1;
    }//if
	SDL_BlitSurface(renderTxt,NULL,ecran,&rectDst);
	SDL_FreeSurface(renderTxt);
}



/*****************************
* permet de dessiner un label
******************************/
void SDL_widgets::drawLabel(int x1,int y1,std::string txt,TTF_Font* font, SDL_Color fgC){
//corps de la boite
	renderTxt=TTF_RenderText_Blended(font,txt.c_str(),fgC);
    //centrage du texte
    rectDst.x=x1;
	rectDst.y=y1;
	SDL_BlitSurface(renderTxt,NULL,ecran,&rectDst);
	SDL_FreeSurface(renderTxt);
}




/*********************************
* Dessiner un Slider
**********************************/
void SDL_widgets::drawSlider(int x1,int y1,int w,int h, TTF_Font* font ,int bgr, int bgg, int bgb, int bga, SDL_Color fgC,int minVal, int maxVal, int CurVal, bool show){
//cadre slider
rectangleRGBA(ecran,x1+20,y1,x1+(w-40),y1+h,bgr,bgg,bgb,bga);

//fleches slider
filledTrigonRGBA(ecran,x1,y1+(h/2),x1+17,y1,x1+17,y1+h,bgr,bgg,bgb,bga);
filledTrigonRGBA(ecran,x1+w-20,y1+(h/2),x1+w-37,y1,x1+w-37,y1+h,bgr,bgg,bgb,bga);
//graduation

//barre
double pas=((w-42)/(maxVal-minVal))-1;

roundedBoxRGBA(ecran,(x1+22),y1+2,((x1+22)+(pas*(CurVal-minVal))),y1+h-2,2,bgr,bgg,bgb,bga);

//std::cout<<"(x1+20)+(pas*(CurVal-minVal))"<<std::endl;
//std::cout<<"pas :"<<pas<<std::endl;
//std::cout<<"curval :"<<CurVal<<" x2:"<<((x1+20)+(pas*(CurVal-minVal)))<<std::endl;


//valeur actuelle
if (show){
    std::ostringstream sbs;sbs<<CurVal;
    renderTxt=TTF_RenderText_Blended(font,sbs.str().c_str(),fgC);
    rectDst.x=x1+20+((w-20)/2);
	rectDst.y=(y1+(h/2))-((renderTxt->h)/2);
    SDL_BlitSurface(renderTxt,NULL,ecran,&rectDst);
    SDL_FreeSurface(renderTxt);
        }
}



/*********************************
* Dessiner un ProgressBar
**********************************/
void SDL_widgets::drawProgressBar(int x1,int y1,int w,int h, TTF_Font* font ,int bgr, int bgg, int bgb, int bga, SDL_Color fgC,int minVal, int maxVal, int CurVal, bool percent){
//cadre slider
rectangleRGBA(ecran,x1,y1,x1+w,y1+h,bgr,bgg,bgb,bga);

//barre
double pas=floor(w/(maxVal-minVal));

roundedBoxRGBA(ecran,x1+2,y1+2,((x1+2)+(pas*(CurVal-minVal))),y1+(h-2),2,bgr,bgg,bgb,bga);

//valeur actuelle
std::ostringstream sbs;sbs<<CurVal;
std::string affichage=sbs.str();
if (percent==true){affichage=sbs.str()+"%";} else {affichage=sbs.str();}
    renderTxt=TTF_RenderText_Blended(font,affichage.c_str(),fgC);
    rectDst.x=x1+(w/2);
	rectDst.y=(y1+(h/2))-((renderTxt->h)/2);
    SDL_BlitSurface(renderTxt,NULL,ecran,&rectDst);
    SDL_FreeSurface(renderTxt);
}




/*****************************
* Dessine une check box
******************************/
void SDL_widgets::drawCheckBox(int x1,int y1,TTF_Font* font ,std::string txt,int bgr, int bgg, int bgb, int bga, SDL_Color fgC,bool checked){
//cadre
rectangleRGBA(ecran,x1,y1,x1+30,y1+30,bgr,bgg,bgb,bga);
//texte
    renderTxt=TTF_RenderText_Blended(font,txt.c_str(),fgC);
    rectDst.x=x1+40;
	rectDst.y=y1+((30-renderTxt->h)/2);
    SDL_BlitSurface(renderTxt,NULL,ecran,&rectDst);
    SDL_FreeSurface(renderTxt);
//checked
if (checked){
    lineRGBA(ecran,x1+5,y1+5,x1+20,y1+25,bgr,bgg,bgb,bga);
    lineRGBA(ecran,x1+20,y1+25,x1+25,y1+15,bgr,bgg,bgb,bga);
    lineRGBA(ecran,x1+6,y1+5,x1+21,y1+24,bgr,bgg,bgb,bga);
    lineRGBA(ecran,x1+19,y1+24,x1+24,y1+15,bgr,bgg,bgb,bga);
    }
}


SDL_widgets::~SDL_widgets()
{}
