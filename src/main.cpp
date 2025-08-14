#include "GameLoop.h"
#include "Game.h"
#include "CLI.h"


int main() {
    CLI cli;
    GameLoop<CLI> gameLoop(cli);
    gameLoop.run();
    return 0;
}
