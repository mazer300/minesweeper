#include "Cell.h"


Cell::Cell(unsigned int const row, unsigned int const col) : row(row), col(col), state(0), mine(false), flag(false),
                                                             is_open(false) {
}

int Cell::getState() const { return state; }
void Cell::setState(int const state) { this->state = state; }

bool Cell::isMine() const { return mine; }
void Cell::setMine(bool const mine) { this->mine = mine; }

bool Cell::isOpen() const { return is_open; }
void Cell::setOpen(bool const is_open) { this->is_open = is_open; }

bool Cell::isFlag() const { return flag; }
void Cell::setFlag(bool const flag) { this->flag = flag; }

std::string Cell::to_str() const {
    if (!is_open) {
        return "[ ]";
    }
    if (flag) {
        return "[~]";
    }
    if (mine) {
        return "[*]";
    }
    return "[" + std::to_string(state) + "]";
}
