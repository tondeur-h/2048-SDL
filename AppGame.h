/*
 * AppGame.h
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



#ifndef APPGAME_H_
#define APPGAME_H_

#include "globalVars.h"

namespace game {

/*
 *
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
	void drawInsideGrid();
	void drawBtn(int x1,int y1,int w,int h,std::string txt,TTF_Font* font, int bgr, int bgg, int bgb, int bga, SDL_Color fgC, bool shadow, bool round, bool center);
	int mouseConvertPos(int x,int y);
	void newGame();
	void newChallenge();
};

} /* namespace game */

#endif /* APPGAME_H_ */
