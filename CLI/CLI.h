#ifndef CLI_H
#define CLI_H

#include <string>
#include "../include/GameLoop.h"
#include <unordered_map>

class CLI {
public:
    CLI();

    explicit CLI(Game *game);

    void print(std::string message);

    void printField();

    Command getCommand();

    std::tuple<int, int> getCoords() const;

    void getDifficulty(unsigned int &number_of_rows, unsigned int &number_of_cols, unsigned int &number_of_mines);

    void setGame(Game *game);

private:
    void clearScreen();
    Game *game;
    std::unordered_map<int, Command> command_map;
};

#endif //GAME_H
