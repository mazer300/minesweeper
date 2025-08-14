cmake_minimum_required(VERSION 3.10)
project(minesweeper)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES "src/*.cpp" "src/*.h")

add_executable(minesweeper
        src/main.cpp
        src/Cell.h
        src/Cell.cpp
        src/Field.cpp
        src/Field.h
        src/Game.cpp
        src/Game.h
        src/GameLoop.h
        src/CLI.cpp
        src/CLI.h
)

