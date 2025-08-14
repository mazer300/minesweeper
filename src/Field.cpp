#include "Field.h"
#include <iostream>
#include <ctime>

Field::Field(unsigned int const number_rows, unsigned int const number_columns,
             unsigned int const number_mines) : number_of_rows(number_rows), number_of_columns(number_columns),
                                                number_of_mines(number_mines), state_of_game(true) {
    // Создание поля
    for (size_t i = 0; i < number_of_rows; i++) {
        std::vector<Cell> tmp;
        for (size_t j = 0; j < number_of_columns; j++) {
            tmp.push_back(Cell(i, j));
        }
        this->cells.push_back(tmp);
    }
    srand(time(NULL));
    // Расстановка мин
    unsigned int counter_mines = 0;
    while (counter_mines < number_of_mines) {
        unsigned int x = rand() % number_of_rows;
        unsigned int y = rand() % number_of_columns;

        if (cells[x][y].isMine() == false) {
            cells[x][y].setMine(true);
            counter_mines++;
        }
    }

    // Подсчёт количества мин для каждой клетки
    for (long int i = 0; i < number_of_rows; i++) {
        for (long int j = 0; j < number_of_columns; j++) {
            int count = 0;
            for (int z = -1; z < 2; z++) {
                for (int w = -1; w < 2; w++) {
                    long int const coord_x = i - z;
                    long int const coord_y = j - w;
                    if (coord_x == -1 || coord_y == -1 || coord_x == number_of_rows || coord_y ==
                        number_of_columns) {
                        continue;
                    }
                    if (cells[coord_x][coord_y].isMine()) count++;
                }
            }
            this->cells[i][j].setState(count);
        }
    }
}

void Field::putFlag(unsigned int const row, unsigned int const col) {
    if (cells[row][col].isOpen() && cells[row][col].isFlag() == false) {
        return;
    }
    if (!cells[row][col].isFlag()) {
        cells[row][col].setFlag(true);
        cells[row][col].setOpen(true);
    } else {
        cells[row][col].setOpen(false);
        cells[row][col].setFlag(false);
    }
}

void Field::attack(unsigned int const row, unsigned int const col) {
    if (!(cells[row][col].isMine() <= cells[row][col].isFlag())) {
        state_of_game = false;
        openMines();
    } else {
        int value = cells[row][col].getState();
        if (value == 0) {
            openMoreCells(row, col);
        }
        cells[row][col].setOpen(true);
    }
}

void Field::openMines() {
    for (size_t i = 0; i < number_of_rows; i++) {
        for (size_t j = 0; j < number_of_columns; j++) {
            if (cells[i][j].isMine()) {
                cells[i][j].setOpen(true);
            }
        }
    }
}

void Field::openMoreCells(unsigned int const row, unsigned int const col) {
    if (col < 0 || row < 0 || row >= number_of_rows || col >= number_of_columns || cells[row][col].isOpen() == true) {
        return;
    }
    cells[row][col].setOpen(true);
    if (cells[row][col].getState() == 0) {
        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                openMoreCells(row + i, col + j);
            }
        }
    }
}

bool Field::getState() {
    return state_of_game;
}

bool Field::isWin() {
    if (!state_of_game) {
        return false;
    }
    int count = 0;
    for (std::vector<Cell> row: cells) {
        for (Cell cell: row) {
            if (cell.isOpen() && !cell.isMine()) {
                count++;
            }
        }
    }
    if (count == number_of_rows * number_of_columns - number_of_mines) {
        return true;
    }
    return false;
}

void Field::print() {
    for (size_t i = 0; i < number_of_rows; i++) {
        for (size_t j = 0; j < number_of_columns; j++) {
            std::cout << cells[i][j].print() << " ";
        }
        std::cout << '\n';
    }
}

unsigned int Field::getRows(){ return number_of_rows; }
unsigned int Field::getCols(){ return number_of_columns; }
Cell& Field::getCell(int row, int col){ return cells[row][col]; }
unsigned int Field::getMines(){
    
}