#ifndef CELL_H
#define CELL_H

#include <string>

class Cell {
public:
    Cell(unsigned int row, unsigned int col);

    int getState();

    void setState(int state);

    bool isMine();

    void setMine(bool mine);

    bool isOpen();

    void setOpen(bool is_open);

    bool isFlag();

    void setFlag(bool flag);

    std::string print();

private:
    unsigned int row;
    unsigned int col;
    int state;
    bool mine;
    bool flag;
    bool is_open;
};


#endif //CELL_H
