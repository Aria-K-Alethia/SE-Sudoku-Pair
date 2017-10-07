cmake_minimum_required(VERSION 3.8)
project(SE_Sudoku)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES InputHandler.h InputHandler.cpp Output.h Output.cpp OutputHandler.h
        OutputHandler.cpp SESudoku.cpp Sudoku.h Sudoku.cpp stdafx.h stdafx.cpp)
add_executable(SE_Sudoku ${SOURCE_FILES})