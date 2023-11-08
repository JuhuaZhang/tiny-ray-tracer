cmake_minimum_required(VERSION 3.22)
project(tiny-ray-tracer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# optimization
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -pedantic")

include_directories(include)

file(GLOB SOURCES "src/*.cpp")
file(GLOB LIB_SOURCES "lib/*.cpp")

add_executable(tiny-ray-tracer ${SOURCES} ${LIB_SOURCES})

# add_library(lib SHARED ${LIB_SOURCES})
# add_executable(tiny-ray-tracer ${SOURCES})
# target_link_libraries(tiny-ray-tracer lib)

