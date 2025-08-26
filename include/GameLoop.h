#ifndef GAMELOOP_H
#define GAMELOOP_H

#include "Game.h"
#include <tuple>

enum Command {
    Attack,
    PutFlag,
    Exit,
    Restart
};


template<typename Interface>
class GameLoop {
public:
    GameLoop(Interface& interface) : interface(interface), flag(false), game(0, 0, 0) {
    }

    void run() {
        while (true) {
            menu();
            if (flag) {
                interface.print("До свидания\n");
                return;
            }
            gameLoop();
        }
    }

private:
    void menu() {
        unsigned int number_of_rows, number_of_cols, number_of_mines;
        interface.getDifficulty(number_of_rows, number_of_cols, number_of_mines);
        if (number_of_rows == -1) {
            flag = true;
            return;
        }
        game = Game(number_of_rows, number_of_cols, number_of_mines);
        interface.setGame(&game);
    }

    void gameLoop() {
        while (true) {
            StateGame state = game.getState(); // Сохраняем состояние

            if (state == StateGame::Win || state == StateGame::Lose) {
                interface.printField();
                interface.showEndGameDialog(state == StateGame::Win);

                // Обработка выбора после окончания игры
                switch (interface.getCommand()) {
                    case Command::Restart:
                        game = Game(game.getRows(), game.getCols(), game.getNumberOfMines());
                        interface.setGame(&game);
                        continue; // Перезапускаем игру
                    case Command::Exit:
                        return; // Выход в меню
                    default:
                        continue;
                }
            }
            else {
                printField();

                std::tuple<int, int> coords;
                switch (interface.getCommand()) {
                    case Command::Attack:
                        coords = interface.getCoords();
                        game.attack(std::get<0>(coords), std::get<1>(coords));
                        break;
                    case Command::PutFlag:
                        coords = interface.getCoords();
                        game.putFlag(std::get<0>(coords), std::get<1>(coords));
                        break;
                    case Command::Exit:
                        return;
                    case Command::Restart:
                        game = Game(game.getRows(), game.getCols(), game.getNumberOfMines());
                        interface.setGame(&game);
                        break;
                    default:
                        break;
                }
            }
        }
    }

    void printField() {
        interface.printField();
    }

    bool flag;
    Game game;
    Interface& interface;
};


#endif //GAMELOOP_H