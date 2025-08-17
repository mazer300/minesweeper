#include "../GUI/GUI.h"
#include <QApplication>
#include "../include/GameLoop.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    GUI gui;
    GameLoop<GUI> gameLoop(gui);
    gui.show();
    gameLoop.run();

    return a.exec();
}

/*#include "../CLI/CLI.h"
#include "../include/GameLoop.h"

int main(int argc, char *argv[]) {


    CLI cli;
    GameLoop<CLI> gameLoop(cli);
    gameLoop.run();

    return 0;
}*/
