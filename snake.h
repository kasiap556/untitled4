#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <list>

#include "screen.h"
#include "winsys.h"

enum gameState { PAUSE, PLAY, HELP, GAME_OVER };
enum direction { LEFT, UP, DOWN, RIGHT };

class CSnake : public CFramedWindow {

    private:
        list<CPoint> snake;
        CPoint apple;
        
        long clock;
        
        gameState state;
        direction dir;
        
        void restart();
        void spawnApple();

    public:
        CSnake(CRect r, char _c = ' ');
        
        void paint();
        bool handleEvent(int c);
};

#endif
