#ifndef CLI_H
#define CLI_H

#include <iostream>
#include <string>
#include <functional>
#include "GameLoop.h"

class CLI{
    void print(std::string message);
    Command getCommand();
    std::tuple<int,int> getCoords();
    void getGame(unsigned int& number_of_rows, unsigned int& number_of_cols, unsigned int& number_of_mines);
};

#endif //GAME_H
