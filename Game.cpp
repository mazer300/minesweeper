#include "Game.h"
#include <iostream>

Game::Game(unsigned int const number_of_rows, unsigned int const number_of_columns,
           unsigned int const number_of_mines) : number_of_rows(number_of_rows),
                                                 number_of_columns(number_of_columns),
                                                 number_of_mines(number_of_mines),
                                                 field(Field(number_of_rows, number_of_columns, number_of_mines)) {
}

void Game::gameLoop() {
    while (true) {
        if (field.isWin()) {
            std::cout << "WIN!!!\n";
            field.print();
            break;
        }
        if (field.getState() == false) {
            std::cout << "Game Over!\n";
            field.print();
            break;
        }
        field.print();

        std::cout << "Enter your choose\nAttack - 1\nPut flag - 2\n";
        int value, x, y;
        std::cin >> value;
        std::cout << "(x,y):";
        std::cin >> x >> y;
        switch (value) {
            case 1:
                field.attack(x,y);
                break;
            case 2:
                field.putFlag(x, y);
                break;
        }
    }
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

unsigned int Game::getRows(){ return field.getRows(); }
unsigned int Game::getCols(){ return field.getCols(); }
Cell& Game::getCell(int row, int col) { return field.getCell(row, col); }