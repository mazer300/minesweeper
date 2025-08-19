#include "../include/Field.h"
#include <iostream>
#include <ctime>

Field::Field(unsigned int const number_rows, unsigned int const number_columns,
             unsigned int const number_mines,unsigned int const first_attack_x,unsigned int const first_attack_y) : number_of_rows(number_rows), number_of_columns(number_columns),
                                                number_of_mines(number_mines), state_of_game(true), count_flags(0) {
    // Создание поля
    for (size_t i = 0; i < number_of_rows; i++) {
        std::vector<Cell> tmp;
        for (size_t j = 0; j < number_of_columns; j++) {
            tmp.emplace_back(Cell(i, j));
        }
        this->cells.push_back(tmp);
    }

    // Расстановка мин
    srand(time(nullptr));
    unsigned int counter_mines = 0;
    while (counter_mines < number_of_mines) {
        unsigned int x = rand() % number_of_rows;
        unsigned int y = rand() % number_of_columns;

        if (cells[x][y].isMine() == false && !(x==first_attack_x && y==first_attack_y)) {
            cells[x][y].setMine(true);
            counter_mines++;
        }
    }

    // Подсчёт количества мин для каждой клетки
    for (long int i = 0; i < number_of_rows; i++) {
        for (long int j = 0; j < number_of_columns; j++) {
            int count = 0;
            for (int z = -1; z <= 1; z++) {
                for (int w = -1; w <= 1; w++) {
                    long int const coord_x = i - z;
                    long int const coord_y = j - w;
                    if (isValidCoord(coord_x, coord_y) && cells[coord_x][coord_y].isMine()) {
                        count++;
                    }
                }
            }
            this->cells[i][j].setState(count);
        }
    }
}

void Field::putFlag(unsigned int const row, unsigned int const col) {
    // Клетка уже открыта
    if (cells[row][col].isOpen() && cells[row][col].isFlag() == false) {
        return;
    }

    // Поставить флаг
    if (!cells[row][col].isFlag()) {
        cells[row][col].setFlag(true);
        cells[row][col].setOpen(true);
        count_flags++;
    } else {
        // Убрать флаг
        cells[row][col].setOpen(false);
        cells[row][col].setFlag(false);
        count_flags--;
    }
}

void Field::attack(unsigned int const row, unsigned int const col) {
    // Круговая атака по открытой клетке
    if (cells[row][col].isOpen() && !cells[row][col].isFlag()) {
        roundAttack(row, col);
    }

    if (cells[row][col].isMine() && !cells[row][col].isFlag()) {
        state_of_game = false;
        openMines();
    } else {
        if (cells[row][col].getState() == 0) {
            openMoreCells(row, col);
        }
        cells[row][col].setOpen(true);
    }
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

bool Field::getState() { return state_of_game; }
unsigned int Field::getRows() { return number_of_rows; }
unsigned int Field::getCols() { return number_of_columns; }
Cell &Field::getCell(unsigned int const row, unsigned int const col) { return cells[row][col]; }
long int Field::getMines() { return (long int) number_of_mines - (long int) count_flags; }
unsigned int Field::getNumberOfMines() { return number_of_mines; }

void Field::roundAttack(unsigned int const row, unsigned int const col) {
    // Проверка количества флагов вокруг клетки
    int count = 0;
    for (int z = -1; z <= 1; z++) {
        for (int w = -1; w <= 1; w++) {
            long int const coord_x = row + z;
            long int const coord_y = col + w;
            if (isValidCoord(coord_x, coord_y) && cells[coord_x][coord_y].isFlag()) {
                count++;
            }
        }
    }

    // Атака клеток вокруг
    if (count == cells[row][col].getState()) {
        for (int z = -1; z <= 1; z++) {
            for (int w = -1; w <= 1; w++) {
                long int const coord_x = row + z;
                long int const coord_y = col + w;
                if (isValidCoord(coord_x, coord_y) && !cells[coord_x][coord_y].isOpen()) {
                    attack(row + z, col + w);
                }
            }
        }
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

// Расширение нуля
void Field::openMoreCells(unsigned int const row, unsigned int const col) {
    if (!isValidCoord((long int) row, (long int) col) || cells[row][col].isOpen()) {
        return;
    }
    cells[row][col].setOpen(true);
    if (cells[row][col].getState() == 0) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                openMoreCells(row + i, col + j);
            }
        }
    }
}

bool Field::isValidCoord(long int const row, long int const col) {
    if (col < 0 || row < 0 || row >= number_of_rows || col >= number_of_columns) return false;
    return true;
}
