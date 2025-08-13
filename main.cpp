#include "GameLoop.h"


int main() {
    Game game(9,9,2);
    GameLoop gameLoop(game);
    gameLoop.run();
    return 0;
}
