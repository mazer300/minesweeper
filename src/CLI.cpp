#include "CLI.h"

CLI::CLI() : game(nullptr) {
}

CLI::CLI(Game *game) : game(game) {
}


void CLI::print(std::string message){
    std::cout << message;
}

void CLI::printField() {
    for (unsigned int i = 0; i < game->getCols() + 1; i++) {
        if (i < 10) {
            std::cout << " " << i << " ";
        } else if (i < 100) {
            std::cout << i << " ";
        } else {
            std::cout << i;
        }
    }
    std::cout << '\n';
    for (unsigned int i = 0; i < game->getRows(); i++) {
        if (i + 1 < 10) {
            std::cout << " " << i + 1 << " ";
        } else if (i + 1 < 100) {
            std::cout << i + 1 << " ";
        } else {
            std::cout << i + 1;
        }

        for (unsigned int j = 0; j < game->getCols(); j++) {
            Cell cell = game->getCell(i, j);
            if (!cell.isOpen()) {
                std::cout << "[ ]";
                continue;
            }
            if (cell.isFlag()) {
                std::cout << "[~]";
                continue;
            }
            if (cell.isMine()) {
                std::cout << "[*]";
                continue;
            }
            std::cout << "[" + std::to_string(cell.getState()) + "]";
        }
        std::cout << '\n';
    }
}

Command CLI::getCommand() {
    int command;
    while (1) {
        std::cout << "Введите команду 1 - атака, 2 - поставить флаг, 5 - выход, 6 - начать сначала:";
        std::cin >> command;
        switch (command) {
            case 1:
                return Command::Attack;

            case 2:
                return Command::PutFlag;
            case 5:
                return Command::Exit;
            case 6:
                return Command::Restart;
            default:
                std::cout << "Неверное значение, попробуйте снова\n";
                break;
        }
    }
}

std::tuple<int, int> CLI::getCoords() {
    long int x, y;
    while (1) {
        std::cout << "Введите координаты (x y): ";
        std::cin >> x >> y;
        if (x >= 1 && y >= 1 && x <= game->getCols() && y <= game->getRows()) {
            return std::make_tuple(y - 1, x - 1);
        } else {
            std::cout << "Неверное значение, попробуйте снова\n";
        }
    }
}

void CLI::getDifficulty(unsigned int &number_of_rows, unsigned int &number_of_cols, unsigned int &number_of_mines) {
    int difficulty;
    while (1) {
        std::cout <<
                "Выберете режим сложности\n 1 - Лёгкий(9х9, 10 мин)\n 2 - Средний(16х16, 40 мин)\n 3 - Тяжёлый(30х16, 99 мин)\n 4 - Пользовательский\n 5 - Выход\n";
        std::cin >> difficulty;
        switch (difficulty) {
            case 1:
                number_of_rows = 9;
                number_of_cols = 9;
                number_of_mines = 10;
                return;

            case 2:
                number_of_rows = 16;
                number_of_cols = 16;
                number_of_mines = 40;
                return;

            case 3:
                number_of_rows = 16;
                number_of_cols = 30;
                number_of_mines = 99;
                return;

            case 4:
                long int rows, cols, mines;
                while (1) {
                    std::cout << "Введите ширину, высоту, количество мин\n(Ш В К): ";
                    std::cin >> cols >> rows >> mines;
                    if (rows > 0 && cols > 0 && mines > 0) {
                        number_of_rows = rows;
                        number_of_cols = cols;
                        number_of_mines = mines;
                        return;
                    } else {
                        std::cout << "Неверное значение, попробуйте снова\n";
                    }
                }

            case 5:
                number_of_rows = -1;
                return;

            default:
                std::cout << "Неверное значение, попробуйте снова\n";
                break;
        }
    }
}

void CLI::setGame(Game *game) {
    this->game = game;
}
