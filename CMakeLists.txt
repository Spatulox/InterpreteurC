cmake_minimum_required(VERSION 3.23)
project(InterpreteurC C)

set(CMAKE_C_STANDARD 11)

add_executable(
        InterpreteurC main.c
        src/c_files/log.c
        src/includes_h/log.h
        src/c_files/file.c
        src/includes_h/file.h)
