#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include "Cell.h"


class Field {
public:
    Field(unsigned int number_rows, unsigned int number_columns, unsigned int number_mines, unsigned int first_attack_x,
          unsigned int first_attack_y);

    void putFlag(unsigned int row, unsigned int col);

    void attack(unsigned int row, unsigned int col);

    bool isWin();

    bool getState();

    unsigned int getRows();

    unsigned int getCols();

    Cell &getCell(unsigned int row, unsigned int col);

    long int getMines();

    unsigned int getNumberOfMines();

private:
    bool isValidCoord(long int row, long int col);

    void roundAttack(unsigned int row, unsigned int col);

    void openMines();

    void openMoreCells(unsigned int row, unsigned int col);

    bool state_of_game;
    unsigned int number_of_rows;
    unsigned int number_of_columns;
    unsigned int number_of_mines;
    std::vector<std::vector<Cell> > cells;
    unsigned int count_flags;
};


#endif //FIELD_H
