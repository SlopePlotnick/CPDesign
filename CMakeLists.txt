cmake_minimum_required(VERSION 3.26)
project(Compiler)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp
        GA.cpp
        LA.cpp
        LA.h
        GA.h)
add_executable(Compiler ${SOURCE_FILES})
