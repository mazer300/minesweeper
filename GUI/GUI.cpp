#include "GUI.h"

GUI::GUI(QWidget *parent) : QMainWindow(parent), game(nullptr) {
    // Реализация конструктора
}

GUI::GUI(QWidget *parent, Game *game) : QMainWindow(parent), game(game) {
    // Реализация конструктора
}

void GUI::print(std::string message) {
    // Implementation for print
}

void GUI::printField() {
    // Implementation for printField
}

Command GUI::getCommand() {
    return Command::Attack;
}

std::tuple<int, int> GUI::getCoords() const {
    return std::make_tuple(1,1);
}

void GUI::getDifficulty(unsigned int &number_of_rows, unsigned int &number_of_cols, unsigned int &number_of_mines) {
    QPushButton button("Hello world!", nullptr);
    button.resize(20, 10);
    button.show();
}

void GUI::setGame(Game *game) {
    this->game = game;
}