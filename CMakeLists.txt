cmake_minimum_required(VERSION 3.22.2)

project(asmVM)

set(EXECUTABLE_OUTPUT_PATH ./bin)

set(CMAKE_CXX_STANDART 17)
set(CMAKE_CXX_STANDART_REQUIRED ON)

set(SRC main.cpp memory.hpp operations.hpp registers.hpp utils.hpp)

add_executable(${PROJECT_NAME} ${SRC})
