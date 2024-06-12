cmake_minimum_required(VERSION 3.12)
project(winerosetta)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_library(winerosetta SHARED src/winerosetta.cpp)
