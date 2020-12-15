cmake_minimum_required(VERSION 3.15)
project(ECE209Prog2 C)

set(CMAKE_C_STANDARD 90)

set(CMAKE_C_FLAGS "-Wall -pedantic")

add_executable(ECE209Prog2 main.c futo.c)