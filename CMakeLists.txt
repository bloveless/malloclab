cmake_minimum_required(VERSION 3.3)
project(lab05_malloc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    clock.c
    clock.h
    config.h
    fcyc.c
    fcyc.h
    fsecs.c
    fsecs.h
    ftimer.c
    ftimer.h
    mdriver.c
    memlib.c
    memlib.h
    mm.c
    mm.h)

add_executable(mdriver ${SOURCE_FILES})