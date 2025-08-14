#ifndef CELL_H
#define CELL_H

#include <string>

class Cell {
public:
    Cell(unsigned int row, unsigned int col);

    [[nodiscard]] int getState() const ;

    void setState(int state);

    [[nodiscard]] bool isMine() const;

    void setMine(bool mine);

    [[nodiscard]] bool isOpen() const;

    void setOpen(bool is_open);

    [[nodiscard]] bool isFlag() const;

    void setFlag(bool flag);

    [[nodiscard]] std::string to_str() const;

private:
    unsigned int row;
    unsigned int col;
    int state;
    bool mine;
    bool flag;
    bool is_open;
};


#endif //CELL_H
