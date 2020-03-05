cmake_minimum_required(VERSION 3.10)
project(memdmx)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3 -march=native")

add_executable(memdmx memdmx.cpp)
