cmake_minimum_required(VERSION 3.10)
project(memdmx C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-Wall -Wextra -O2 -march=native")

add_executable(memdmx memdmx.c)
