#include "../include/GameLoop.h"
#include "../CLI/CLI.h"

int main() {
    CLI cli;
    GameLoop<CLI> gameLoop(cli);
    gameLoop.run();
    return 0;
}
