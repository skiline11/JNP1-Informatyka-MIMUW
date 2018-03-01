cmake_minimum_required(VERSION 3.8)
project(ClionRozwiazanie)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp decoration.cc helper.cc person.cc)
add_executable(ClionRozwiazanie ${SOURCE_FILES})