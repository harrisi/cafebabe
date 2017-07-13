cmake_minimum_required(VERSION 3.7)
project(cafebabe)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/printer.c
        src/printer.h
        src/cafebabe.c
        src/cafebabe.h
        src/constantpool.h
        src/cafebabetypes.h)

add_executable(cafebabe ${SOURCE_FILES})