#include "Cell.h"


Cell::Cell(unsigned int const row, unsigned int const col) : row(row), col(col), state(0), mine(false), flag(false),
                                                       is_open(false) {
}

int Cell::getState() { return state; }
void Cell::setState(int const state) { this->state = state; }

bool Cell::isMine() { return mine; }
void Cell::setMine(bool const mine) { this->mine = mine; }

bool Cell::isOpen() { return is_open; }
void Cell::setOpen(bool const is_open) { this->is_open = is_open; }

bool Cell::isFlag() { return flag; }
void Cell::setFlag(bool const flag) { this->flag = flag; }

std::string Cell::print() {
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
