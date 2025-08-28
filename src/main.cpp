#ifdef USE_GUI
    #include "../GUI/GUI.h"
    #include <QApplication>
#else
    #include "../CLI/CLI.h"
#endif

#include "../include/GameLoop.h"

int main(int argc, char *argv[]) {
#ifdef USE_GUI
    QApplication a(argc, argv);
    GUI gui;
    GameLoop<GUI> gameLoop(gui);
    gui.show();
    gameLoop.run();
    return a.exec();
#else
    CLI cli;
    GameLoop<CLI> gameLoop(cli);
    gameLoop.run();
    return 0;
#endif
}
