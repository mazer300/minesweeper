#include "../include/Game.h"

Game::Game(unsigned int const number_of_rows, unsigned int const number_of_columns,
           unsigned int const number_of_mines) : number_of_rows(number_of_rows),
                                                 number_of_columns(number_of_columns),
                                                 number_of_mines(number_of_mines),
                                                 field(Field(number_of_rows, number_of_columns, number_of_mines)) {
}

void Game::attack(unsigned int const x, unsigned int const y) {
    field.attack(x, y);
}

void Game::putFlag(unsigned int const x, unsigned int const y) {
    field.putFlag(x, y);
}

StateGame Game::getState() {
    if (field.isWin()) return StateGame::Win;
    if (field.getState()) return StateGame::Round;
    return StateGame::Lose;
}

unsigned int Game::getRows()  { return field.getRows(); }
unsigned int Game::getCols() { return field.getCols(); }
unsigned int Game::getMines() { return field.getMines(); }
Field Game::getField() { return field; }
unsigned int Game::getNumberOfMines() { return field.getNumberOfMines(); }
Cell &Game::getCell(unsigned int const row, unsigned int const col) { return field.getCell(row, col); }
