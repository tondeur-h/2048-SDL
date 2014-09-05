#ifndef SDL_WIDGETS_H
#define SDL_WIDGETS_H

#include "globalVars.h"


class SDL_widgets
{
    public:
        SDL_Rect rectDst;
        SDL_Surface* ecran;
        SDL_Surface* renderTxt;

        SDL_widgets(SDL_Surface* TheScreen);
        void drawBtn(int x1,int y1,int w,int h,std::string txt,TTF_Font* font, int bgr, int bgg, int bgb, int bga, SDL_Color fgC, bool shadow, bool round, bool center);
        void drawSlider(int x1,int y1,int w,int h, TTF_Font* font ,int bgr, int bgg, int bgb, int bga, SDL_Color fgC,int minVal, int maxVal, int CurVal, bool show);
        void drawProgressBar(int x1,int y1,int w, int h, TTF_Font* font, int bgr,int bgg, int bgb,int bga, SDL_Color fgC,int minVal, int maxVal, int Curval, bool percent);
        void drawCheckBox(int x1,int y1, TTF_Font* font ,std::string txt,int bgr, int bgg, int bgb, int bga, SDL_Color fgC,bool checked);
        void drawLabel(int x1, int y1, std::string txt, TTF_Font* font, SDL_Color fgC);
        void drawImage(int x1,int y1,SDL_Surface* img);
        int mouseInBox(int mx, int my, int x1,int y1, int w, int h, int retour);
        virtual ~SDL_widgets();
    protected:
    private:
};

#endif // SDL_WIDGETS_H
