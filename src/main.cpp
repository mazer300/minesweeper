/*#include "../GUI/GUI.h"
#include "../CLI/CLI.h"
#include <QStringConverter>
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QTextStream(stdout).setEncoding(QStringConverter::Utf8);
    QTextStream(stderr).setEncoding(QStringConverter::Utf8);

    GUI gui;
    GameLoop<GUI> game_loop(gui);

    //game_loop.run();
    gui.show();

    return a.exec();
}*/

#include "../include/GameLoop.h"
#include "../CLI/CLI.h"

int main() {
    CLI cli;
    GameLoop<CLI> gameLoop(cli);
    gameLoop.run();
    return 0;
}