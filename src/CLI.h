#ifndef CLI_H
#define CLI_H

#include <iostream>
#include <string>
#include <functional>
#include "GameLoop.h"

class CLI {
public:
    CLI();

    explicit CLI(Game *game);


    void print(std::string message);

    void printField();

    Command getCommand();

    std::tuple<int, int> getCoords();

    void getDifficulty(unsigned int &number_of_rows, unsigned int &number_of_cols, unsigned int &number_of_mines);

    void setGame(Game *game);

private:
    Game *game;
};

#endif //GAME_H
