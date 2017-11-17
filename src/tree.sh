#!/bin/bash

gcc main.c Maze.c UnionFindTree.c --std=c99 --pedantic -Wall -Wextra -Wmissing-prototypes -DNDEBUG -lm -o test