#include "CLI.h"

void CLI::print(std::string message){
    std::cout<<message;
}

Command CLI::getCommand(){
    int command;
    while(1){
        std::cin>>command;
        switch (command)
        {
        case 1:
            return Command::attack;

        case 2:
            return Command::putFlag;
        
        default:
            std::cout<<"Неверное значение, попробуйте снова\n";
            break;
        }
    }
}

std::tuple<int,int> CLI::getCoords(){
    long int x,y;
    while(1){
        std::cout<<"Введите координаты (x y): ";
        std::cin>>x>>y;
        if(x>=1 && y>=1){
            return std::make_tuple(x-1,y-1);
        }else{
            std::cout<<"Неверное значение, попробуйте снова\n";
        }
    }
    
}

void CLI::getGame(unsigned int& number_of_rows, unsigned int& number_of_cols, unsigned int& number_of_mines){
    int difficulty;
    while(1){
        std::cout<<"Выберете режим сложности\n 1 - Лёгкий(9х9, 10 мин)\n 2 - Средний(16х16, 40 мин)\n 3 - Тяжёлый(30х16, 99 мин)\n 4 - Пользовательский\n 5 - Выход";
        std::cin>>difficulty;
        switch (difficulty)
        {
        case 1:
            number_of_rows=9;
            number_of_cols=9;
            number_of_mines=10;
            return;

        case 2:
            number_of_rows=16;
            number_of_cols=16;
            number_of_mines=40;
            return;

        case 3:
            number_of_rows=30;
            number_of_cols=16;
            number_of_mines=99;
            return;

        case 4:
            long int rows,cols,mines;
            while(1){
                std::cout<<"Введите ширину, высоту, количество мин\n(Ш В К): ";
                std::cin>>rows>>cols>>mines;
                if(rows>0 && cols>0 && mines>0){
                    number_of_rows=rows;
                    number_of_cols=cols;
                    number_of_mines=mines;
                    return;
                }else{
                    std::cout<<"Неверное значение, попробуйте снова\n";
                }
            }

        case 5:
            number_of_rows=-1;
            return;

        default:
            std::cout<<"Неверное значение, попробуйте снова\n";
            break;
        }
    }
}