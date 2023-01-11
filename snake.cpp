#include <cstdlib>
#include <ctime>

#include "snake.h"

CSnake::CSnake(CRect r, char _c /*=' '*/) : CFramedWindow(r, _c) { restart(); }

void CSnake::paint() {

    CFramedWindow::paint();
    
    if (state != HELP) {
        gotoyx(apple.y + geom.topleft.y, apple.x + geom.topleft.x);
        printl("O");
    
        for (CPoint segment : snake) {
            gotoyx(segment.y + geom.topleft.y, segment.x + geom.topleft.x);
            printl("#");
        }
    }
    
    if (state == GAME_OVER) {
        gotoyx(geom.topleft.y + 1, geom.topleft.x + 1);
        printl("*GAME OVER!*");
        gotoyx(geom.topleft.y + 2, geom.topleft.x + 1);
        printl("SCORE: %d", snake.size());
        gotoyx(geom.topleft.y + 3, geom.topleft.x + 1);
        printl("Press r to restart");
    }
    
    if (state == PAUSE) {
        gotoyx(geom.topleft.y + 1, geom.topleft.x + 1);
        printl("*GAME PAUSED*");
    }
    
    if (state == HELP) {
        gotoyx(geom.topleft.y + 3, geom.topleft.x + 3);
        printl("h - open help information");
        gotoyx(geom.topleft.y + 4, geom.topleft.x + 3);
        printl("p - toggle pause/play mode");
        gotoyx(geom.topleft.y + 5, geom.topleft.x + 3);
        printl("r - restart game");
        gotoyx(geom.topleft.y + 6, geom.topleft.x + 3);
        printl("arrows - move snake (in play mode) or");
        gotoyx(geom.topleft.y + 7, geom.topleft.x + 3);
        printl("         move window (in pause mode)");
    }
}

bool CSnake::handleEvent(int c) {

    if (state == GAME_OVER) {
        if (c == 'r' || c == 'R') {
            restart();
            return true;
        }
        
        return CFramedWindow::handleEvent(c);
    }
    
    if (state == PLAY) {
        switch (c) {
            case KEY_LEFT:
                if (dir != RIGHT) {
                    dir = LEFT;
                }
                return true;
            case KEY_UP:
                if (dir != DOWN) {
                    dir = UP;
                }
                return true;
            case KEY_DOWN:
                if (dir != UP) {
                    dir = DOWN;
                }
                return true;
            case KEY_RIGHT:
                if (dir != LEFT) {
                    dir = RIGHT;
                }
                return true;
        }
        
        if (++clock > 5 - ((long) snake.size() - 5) % 3) {
            CPoint newFront(snake.front().x, snake.front().y);
        
            if (newFront.x != apple.x || newFront.y != apple.y) {
                snake.pop_back();
            } else {
                spawnApple();
            }
        
            switch (dir) {
                case LEFT:
                    newFront.x -= 1;
                    break;
                case UP:
                    newFront.y -= 1;
                    break;
                case DOWN:
                    newFront.y += 1;
                    break;
                case RIGHT:
                    newFront.x += 1;
                    break;
            }
        
            if (newFront.x <= 0) {
                newFront.x = geom.size.x - 2;
            } else if (newFront.x >= geom.size.x - 1) {
                newFront.x = 1;
            } else if (newFront.y <= 0) {
                newFront.y = geom.size.y - 2;
            } else if (newFront.y >= geom.size.y - 1) {
                newFront.y = 1;
            }
        
            for (CPoint segment : snake) {
                if (newFront.x == segment.x && newFront.y == segment.y) {
                    state = GAME_OVER;
                    return true;
                }
            }
        
            snake.push_front(newFront);
        
            clock = 0;
        }
    }
    
    if (c == 'p' || c == 'P') {
        state = state == PLAY ? PAUSE : PLAY;
        return true;
    }
    
    if (c == 'h' || c == 'H') {
        state = HELP;
        return true;
    }
    
    if (c == 'r' || c == 'R') {
        restart();
        return true;
    }

    if (c == '\t') {
        state = state == PLAY ? PAUSE : state;
    }
    
    if (state != PLAY) {
        return CFramedWindow::handleEvent(c);
    }

    return true;
}

void CSnake::restart() {

    srand(time(NULL));
    snake.clear();
    
    clock = 0;
    state = HELP;
    dir = RIGHT;
     
    for (int i = 4; i >= 0; i--) {
        snake.push_back(CPoint(5 + i, 5));
    }
    
    spawnApple();
}

void CSnake::spawnApple() {
    
    apple.x = rand() % (geom.size.x - 2) + 1;
    apple.y = rand() % (geom.size.y - 2) + 1;
    
    for (CPoint segment : snake) {
        if (apple.x == segment.x && apple.y == segment.y) {
            spawnApple();
        }
    }
}

