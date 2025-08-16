#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include <QMenu>
#include <QThread>
#include "../include/GameLoop.h"
#include "../include/Game.h"
#include <string>


class GUI : public QMainWindow {
    Q_OBJECT

public:
    explicit GUI(QWidget *parent = nullptr);

    GUI(QWidget *parent, Game *game);

    ~GUI() override = default;

    void print(std::string message);

    void printField();

    Command getCommand();

    std::tuple<int, int> getCoords() const;

    void getDifficulty(unsigned int &number_of_rows, unsigned int &number_of_cols, unsigned int &number_of_mines);

    void setGame(Game *game);

private:
    Game *game;
};

#endif // GUI_H
