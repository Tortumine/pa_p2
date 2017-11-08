cmake_minimum_required(VERSION 3.8)
project(pa_p2)

set(CMAKE_C_FLAGS "${CMAKE_ENABLE_C99}")

set(SOURCE_FILES
        src/libs/maze/Maze.c
        src/libs/maze/Maze.h
        src/libs/union-find/UnionFind.h
        src/libs/union-find/UnionFindList.c
        src/libs/union-find/UnionFindTree.c
        src/main.c)

include_directories("${CMAKE_SOURCE_DIR}/src/libs/maze"
        "${CMAKE_SOURCE_DIR}src/libs/union-find")

add_executable(pa_p2 ${SOURCE_FILES})
