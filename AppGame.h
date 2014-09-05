/*
 * AppGame.h
 *
 *  Created on: Aug 22, 2014
 *      Author: herve
 */

#ifndef APPGAME_H_
#define APPGAME_H_

#include "globalVars.h"

namespace game {

/*
 * Classe du jeu
 */
class AppGame {
public:
	AppGame();
	bool initSDL();
	void loopGame();
	virtual ~AppGame();

private:
	void checkBestScore();
	int checkTab();
	void calculateTab(int dir);
	void drawTab();
	void drawGrid();
	void drawTools();
	void drawBtn(int x1,int y1,int w,int h,std::string txt, TTF_Font* font ,int bgr, int bgg, int bgb, int bga, SDL_Color fgC, bool shadow, bool round, bool center);
	void drawInsideGrid();
	int mouseConvertPos(int x,int y);
	void newGame();
	void newChallenge();
	void say(std::string txt);
};

} /* namespace game */

#endif /* APPGAME_H_ */
