#include "GameLoop.h"
#include "CLI.h"
#include <windows.h>

int main() {
    SetConsoleOutputCP(CP_UTF8);
    CLI cli;
    GameLoop<CLI> gameLoop(cli);
    gameLoop.run();
    return 0;
}
