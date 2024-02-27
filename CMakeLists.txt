cmake_minimum_required(VERSION 3.26)
project(BakalarskaPraca)

set(CMAKE_CXX_STANDARD 23)

add_executable(BakalarskaPraca main.cpp
        Matrix.h
        FileReader.h
        Algorithms.h
        App.h
        Generator.h
        FileWriter.h
)
