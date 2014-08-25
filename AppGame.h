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
 *
 */
class AppGame {
public:
	AppGame();
	bool initSDL();
	void loopGame();
	virtual ~AppGame();

private:
	//void frameRate(); //not used
	int checkTab();
	void calculateTab(int dir);
	void drawTab();
	void drawGrid();
	void drawTools();
	void drawInsideGrid();
	int mouseConvertPos(int x,int y);
	std::string get_file(std::string file);
	void newGame();
};

} /* namespace game */

#endif /* APPGAME_H_ */
