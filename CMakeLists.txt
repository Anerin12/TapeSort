cmake_minimum_required(VERSION 3.16)
project(TapeSortProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(APP_SOURCES
    src/main.cpp
)

set(CORE_SOURCES
    src/Tape.cpp 
    src/TapeSort.cpp 
    utils/Config.cpp 
    utils/generator.cpp
    utils/Logger.cpp

)

add_executable(TapeSortApp ${APP_SOURCES} ${CORE_SOURCES})

target_include_directories(TapeSortApp PRIVATE headers)

configure_file(config/config.txt config.txt COPYONLY)


include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.15.0 
)
FetchContent_MakeAvailable(Catch2)

set(TEST_SOURCES
    tests/test_Tape.cpp
    tests/test_TapeSort.cpp 
)

add_executable(RunALLTests ${TEST_SOURCES} ${CORE_SOURCES})

target_include_directories(RunALLTests PRIVATE headers config)

target_link_libraries(RunALLTests PRIVATE Catch2::Catch2WithMain)