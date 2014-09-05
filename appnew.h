#ifndef APPNEW_H
#define APPNEW_H

#include "globalVars.h"

class AppNew
{
    public:
        AppNew();
        virtual ~AppNew();
    protected:
    private:
        void drawScreen();
        void resetBestScore();
        void loopNew();
        void setGOAL(int nbc);
};

#endif // APPNEW_H
