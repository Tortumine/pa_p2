#!/bin/bash

gcc main.c Maze.c UnionFindList.c --std=c99 --pedantic -Wall -Wextra -Wmissing-prototypes -DNDEBUG -lm -o list
