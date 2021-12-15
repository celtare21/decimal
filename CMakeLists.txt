cmake_minimum_required(VERSION 3.21)
project(C_project C)

set(CMAKE_C_STANDARD 99)

add_executable(C_project main.c sds.c sds.h sds.h map.c map.h)
