cmake_minimum_required(VERSION 3.16)
project(TapeSortProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(headers)
include_directories(config)

set(SOURCES
    src/main.cpp
    src/render.cpp
    src/Tape.cpp
    src/TapeSort.cpp
    config/Config.cpp
)

add_executable(TapeSortApp ${SOURCES})

configure_file(config/config.txt config.txt COPYONLY)