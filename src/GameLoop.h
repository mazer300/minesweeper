#ifndef GAMELOOP_H
#define GAMELOOP_H

#include "Game.h"
#include <functional>


enum Command{
    attack,
    putFlag
};


template<typename Interface>
class GameLoop {
public:
    GameLoop(Interface interface) : interface(interface), flag(false){
        game=Game(0,0,0);
    }

    void run() {
        while(1){
            menu();
            if(flag){
                interface.print("Досвидания\n");
                return;
            }
            gameLoop();
        }
    }

private:
    void menu(){
        unsigned int& number_of_rows, number_of_cols, number_of_mines;
        interface.getGame(number_of_rows, number_of_cols, number_of_mines);
        if(number_of_rows==-1){
            flag=true;
            return;
        }
        game=Game(number_of_rows,number_of_cols,number_of_mines);
    }

    void gameLoop(){
        while (true) {
            switch (game.getState())
            {
            case StateGame::Win:
                interface.print("WIN!!!\n");
                printField();
                break;

            case StateGame::Lose:
                interface.print("Game Over!\n");
                printField();
                break;

            case StateGame::Round:
                break;
            
            default:
                break;
            }
            
            interface.print("Мины: "+std::to_string(game.getMines())+"\n");
            printField();

            Command command;
            interface.getCommand();
            std::tuple<int,int> coords=interface.getCoords();
            switch (command)
            {
            case Command::attack:
                game.attack(std::get<0>(coords),std::get<1>(coords));
                break;
            case Command::putFlag:
                game.putFlag(std::get<0>(coords),std::get<1>(coords));
                break;

            default:
                break;
            }
        }
    }

    void printField(){
        int x;
    }

    bool flag;
    Game &game;
    Interface interface;
};


#endif //GAMELOOP_H
