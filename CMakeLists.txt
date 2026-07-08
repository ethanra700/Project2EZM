cmake_minimum_required(VERSION 3.16)
project(Project2EZM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(${PROJECT_NAME}
    src/structures/heap.cpp
    src/structures/heap.h
    src/structures/hashmap.cpp
    src/structures/hashmap.h
    src/data/loader.cpp
    src/data/loader.h
    src/data/GymRecord.h
    src/bench/bench.cpp
    src/bench/bench.h
    src/gui/MainWindow.cpp
    src/gui/MainWindow.h
)

target_include_directories(${PROJECT_NAME} PRIVATE
    src/structures
    src/data
    src/bench
    src/gui
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
