cmake_minimum_required(VERSION 3.12)
project(winerosetta)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_library(winerosetta SHARED src/winerosetta.cpp)

project(winerosettaldr)
add_executable(winerosettaldr WIN32 src/loader.cpp)

set_target_properties(winerosetta PROPERTIES PREFIX "")  # Add this line

if(MINGW)
  target_link_options(winerosettaldr PRIVATE -static)
endif()
