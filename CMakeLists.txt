cmake_minimum_required(VERSION 3.10)
project(minesweeper)

set(CMAKE_CXX_STANDARD 17)

# Опция для выбора интерфейса
option(BUILD_GUI "Build with GUI interface" ON)
option(BUILD_CLI "Build with CLI interface" ON)

# Настраиваемый путь к Qt для всех платформ
set(QT_PATH "" CACHE PATH "Path to Qt installation (e.g., C:\\Qt\\6.9.1\\mingw_64 on Windows or /home/user/Qt/6.9.1/gcc_64 on Linux)")

# Проверка, что выбран хотя бы один интерфейс
if(NOT BUILD_GUI AND NOT BUILD_CLI)
    message(FATAL_ERROR "At least one interface (GUI or CLI) must be enabled")
endif()

# Общие исходные файлы
set(COMMON_SOURCES
    src/Cell.cpp
    src/Field.cpp
    src/Game.cpp
    include/Cell.h
    include/Field.h
    include/Game.h
    include/GameLoop.h
)

# Настройка GUI
if(BUILD_GUI)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)
    
    # Устанавливаем путь к Qt если он указан
    if(QT_PATH)
        set(CMAKE_PREFIX_PATH ${QT_PATH})
        message(STATUS "Using Qt path: ${QT_PATH}")
    else()
        # Попробуем найти Qt автоматически
        if(WIN32)
            # Пути по умолчанию для Windows
            set(POSSIBLE_QT_PATHS
                "C:\\Qt\\6.9.1\\mingw_64"
                "C:\\Qt\\6.8.0\\mingw_64"
                "C:\\Qt\\6.7.0\\mingw_64"
            )
        elseif(UNIX AND NOT APPLE)
            # Пути по умолчанию для Linux
            set(POSSIBLE_QT_PATHS
                "/home/${USER}/Qt/6.9.1/gcc_64"
                "/opt/Qt/6.9.1/gcc_64"
                "/usr/local/Qt/6.9.1/gcc_64"
            )
        endif()
        
        foreach(qt_path IN LISTS POSSIBLE_QT_PATHS)
            if(EXISTS "${qt_path}")
                set(CMAKE_PREFIX_PATH "${qt_path}")
                set(QT_PATH "${qt_path}" CACHE PATH "Path to Qt installation" FORCE)
                message(STATUS "Found Qt at: ${qt_path}")
                break()
            endif()
        endforeach()
    endif()
    
    find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
    
    set(GUI_SOURCES
        GUI/GUI.h
        GUI/GUI.cpp
    )
    
    list(APPEND COMMON_SOURCES ${GUI_SOURCES})
endif()

# Настройка CLI
if(BUILD_CLI)
    set(CLI_SOURCES
        CLI/CLI.h
        CLI/CLI.cpp
    )
    
    list(APPEND COMMON_SOURCES ${CLI_SOURCES})
endif()

# Главный исполняемый файл
add_executable(minesweeper
    src/main.cpp
    ${COMMON_SOURCES}
)

# Линковка Qt для GUI
if(BUILD_GUI)
    target_link_libraries(minesweeper
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
    )
    
    # Добавляем определение для компилятора
    target_compile_definitions(minesweeper PRIVATE USE_GUI)
endif()

# Windows-specific deployment
if(WIN32 AND BUILD_GUI)
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

# Установка выходных файлов
install(TARGETS minesweeper RUNTIME DESTINATION bin)