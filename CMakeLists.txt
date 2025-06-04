cmake_minimum_required(VERSION 3.12)
project(SandPile VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(SandPile main.cpp FileWorker.cpp Cell.cpp)
