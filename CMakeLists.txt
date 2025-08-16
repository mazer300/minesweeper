cmake_minimum_required(VERSION 3.10)
project(minesweeper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH "C:\\Qt\\6.9.1\\mingw_64")

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

add_executable(minesweeper
        src/main.cpp
        GUI/GUI.h
        GUI/GUI.cpp
        include/Cell.h
        src/Cell.cpp
        src/Field.cpp
        include/Field.h
        src/Game.cpp
        include/Game.h
        include/GameLoop.h
        CLI/CLI.cpp
        CLI/CLI.h
)

target_link_libraries(minesweeper
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

# Windows-specific deployment
if(WIN32)
    set(DEBUG_SUFFIX "")
    if(MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif()

    get_target_property(QT_QMAKE_EXECUTABLE Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QT_QMAKE_EXECUTABLE}" DIRECTORY)

    add_custom_command(TARGET minesweeper POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:minesweeper>/plugins/platforms/"
            COMMAND ${CMAKE_COMMAND} -E copy
            "${QT_BIN_DIR}/../plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
            "$<TARGET_FILE_DIR:minesweeper>/plugins/platforms/"
    )

    foreach(QT_LIB Core Gui Widgets)
        add_custom_command(TARGET minesweeper POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_BIN_DIR}/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:minesweeper>"
        )
    endforeach()
endif()