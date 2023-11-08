cmake_minimum_required(VERSION 3.24)
project(mp_4cr)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -pedantic") # for final build and run
#set(CMAKE_BUILD_TYPE Debug) # for debug

add_executable(program main.cpp lodepng.cpp vec.cpp)

target_include_directories(program PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
