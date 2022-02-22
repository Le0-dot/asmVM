cmake_minimum_required(VERSION 3.22.2)

project(asmVM)

set(EXECUTABLE_OUTPUT_PATH ./bin)

set(CMAKE_CXX_STANDART 17)
set(CMAKE_CXX_STANDART_REQUIRED ON)

set(SRC src/main.cpp src/memory.hpp src/registers.hpp src/operations.hpp src/utils.hpp
    src/virtual_machine.hpp)

add_executable(${PROJECT_NAME} ${SRC})
