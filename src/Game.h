#ifndef GAME_H
#define GAME_H

#include "Field.h"

enum class StateGame {
    Win,
    Lose,
    Round
};

class Game {
public:
    Game(unsigned int number_of_rows, unsigned int number_of_columns, unsigned int number_of_mines);

    void attack(unsigned int x, unsigned int y);

    void putFlag(unsigned int x, unsigned int y);

    StateGame getState();

    unsigned int getRows();

    unsigned int getCols();

    unsigned int getMines();

    Field getField();

    unsigned int getNumberOfMines();

    Cell &getCell(unsigned int row, unsigned int col);

private:
    unsigned int number_of_rows;
    unsigned int number_of_columns;
    unsigned int number_of_mines;
    Field field;
};


#endif //GAME_H
