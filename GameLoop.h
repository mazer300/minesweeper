#ifndef GAMELOOP_H
#define GAMELOOP_H

#include "Game.h"

class GameLoop {
public:
    GameLoop(Game& game) : game(game){}
    void run() {
        game.gameLoop();
    }

private:
    Game &game;
};


#endif //GAMELOOP_H
