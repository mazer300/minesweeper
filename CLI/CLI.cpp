#include "CLI.h"
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#include <limits>
#else
#include <cstdlib>
#endif

CLI::CLI() : game(nullptr) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); // Устанавливаем UTF-8 для вывода
    SetConsoleCP(CP_UTF8); // Устанавливаем UTF-8 для ввода
#else
    setlocale(LC_ALL, "en_US.UTF-8");  // Для Linux
#endif
    command_map[1] = Command::Attack;
    command_map[2] = Command::PutFlag;
    command_map[5] = Command::Exit;
    command_map[6] = Command::Restart;
}

CLI::CLI(Game *game) : game(game) {
}

void CLI::print(std::string message) {
    std::cout << message;
}

void CLI::clearScreen() {
#ifdef _WIN32
    // Windows-версия

#else
    // Linux/macOS версия
    system("clear");
#endif
}

void CLI::printField() {
    clearScreen();
    std::cout << "Мины: " << game->getMines() << '\n';

    // Вывод номеров столбцов
    for (unsigned int i = 0; i < game->getCols() + 1; i++) {
        if (i < 10) {
            std::cout << " " << i << " ";
        } else if (i < 100) {
            std::cout << i << " ";
        } else {
            std::cout << i;
        }
    }

    // Вывод поля
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
    while (true) {
        std::cout << "Введите команду 1 - атака, 2 - поставить флаг, 5 - выход, 6 - начать сначала:";
        if (!(std::cin >> command)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка! Введите число.\n";
            continue;
        }
        //std::cin >> command;
        if (command_map.find(command) != command_map.end()) {
            return command_map[command];
        } else {
            std::cout << "Неверное значение, попробуйте снова\n";
        }
    }
}

std::tuple<int, int> CLI::getCoords() const {
    long int x, y;
    while (true) {
        std::cout << "Введите координаты (x y): ";
        if (!(std::cin >> x >> y)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка! Введите два числа.\n";
            continue;
        }
        if (x >= 1 && y >= 1 && x <= game->getCols() && y <= game->getRows()) {
            return std::make_tuple(y - 1, x - 1);
        } else {
            std::cout << "Неверное значение, попробуйте снова\n";
        }
    }
}

void CLI::getDifficulty(unsigned int &number_of_rows, unsigned int &number_of_cols, unsigned int &number_of_mines) {
    int difficulty;
    while (true) {
        clearScreen();
        std::cout <<
                "Выберете режим сложности\n 1 - Лёгкий(9х9, 10 мин)\n 2 - Средний(16х16, 40 мин)\n 3 - Тяжёлый(30х16, 99 мин)\n 4 - Пользовательский\n 5 - Выход\n";
        if (!(std::cin >> difficulty)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка! Введите число.\n";
            continue;
        }
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
                while (true) {
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
