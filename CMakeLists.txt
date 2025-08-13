cmake_minimum_required(VERSION 3.31)
project(minesweeper)

set(CMAKE_CXX_STANDARD 20)

add_executable(minesweeper main.cpp
        Cell.h
        Cell.cpp
        Field.cpp
        Field.h
        Game.cpp
        Game.h
        QTInputHandler.cpp
        QTInputHandler.h
        QTRenderer.h
        GameLoop.h
        GameController.cpp
        GameController.h
        GameRenderer.cpp
        GameRenderer.h
        QTRenderer.cpp)