#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include "Cell.h"


class Field {
public:
    Field(unsigned int number_rows, unsigned int number_columns, unsigned int number_mines);


    void putFlag(unsigned int row, unsigned int col);

    void attack(unsigned int row, unsigned int col);

    void openMines();

    void openMoreCells(unsigned int row, unsigned int col);

    bool getState();

    bool isWin();

    void print();

    unsigned int getRows();

    unsigned int getCols();

    Cell &getCell(int row, int col);

    long int getMines();

    unsigned int getNumberOfMines();

private:
    bool state_of_game;
    unsigned int number_of_rows;
    unsigned int number_of_columns;
    unsigned int number_of_mines;
    std::vector<std::vector<Cell> > cells;
    unsigned int count_flags;
};


#endif //FIELD_H
