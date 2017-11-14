#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "Maze.h"
#include "UnionFind.h"

struct maze_t {
    UnionFind *union_Find;
    struct Walls *myWalls;
    size_t number_elements;
    size_t number_inner_walls;
};

//typedef struct walls_t Walls;
typedef struct Walls {
    Coord Cell1;
    Coord Cell2;
    bool wall_between;
}Walls;

//structure representing two adjacent cells, with a wall or not between them.

Maze* mzCreate(size_t size)
{
    int innerWalls;//le nombre de murs intérieurs.
    Maze *myMaze;
    myMaze = malloc(sizeof(Maze));
    myMaze->number_elements = size*size;
    //set up the struct UnionFind
    //myMaze->union_Find = malloc(size * size * sizeof(*myMaze->union_Find));
    
     //unionFind->elements = malloc(n_items * n_items * sizeof(*unionFind->elements));
    
    myMaze->union_Find = ufCreate(size*size);

    //setting up the walls of the maze
    //number of inner walls is size*((size-1)*2)
    innerWalls = size*((size-1)*2);
    myMaze->myWalls = malloc(innerWalls*sizeof(*myMaze->myWalls));
    myMaze->number_inner_walls = innerWalls;
    //setting up walls between adjacent cells. Vertical walls are at the beginning of the array.
    //Horizontal walls are a the end of the arrey
    int setVerWalls = 0;
    int setHorWalls = 0;
    for(size_t i=0;i<(size*size);i++)
        {
            if(i%size < size-1)//set a wall between a cell and it's right neighbour
            {           
            myMaze->myWalls[setVerWalls].Cell1.row=i/size;
            myMaze->myWalls[setVerWalls].Cell1.col=i%size;
            myMaze->myWalls[setVerWalls].Cell2.row=(i+1)/size;
            myMaze->myWalls[setVerWalls].Cell2.col=(i+i)%size;
            myMaze->myWalls[setVerWalls].wall_between=true;
            setVerWalls ++;
            }
            if(i/size < size-1)//set a wall between a cell and it's bottom neighbour
            {           
            myMaze->myWalls[setVerWalls].Cell1.row=i/size;
            myMaze->myWalls[setVerWalls].Cell1.col=i%size;
            myMaze->myWalls[setVerWalls].Cell2.row=(i+size)/size;
            myMaze->myWalls[setVerWalls].Cell2.col=(i+size)%size;
            myMaze->myWalls[setHorWalls + innerWalls/2].wall_between=true;
            setHorWalls ++;
            }
        }
     //Mixing the walls. This way, the walls will be parcoured on a random way.
       struct Walls temp;
        for(int i=0;i<innerWalls;i++)
            { 
                int i2 = rand()%innerWalls;  
                
                temp =  myMaze->myWalls[i];
                myMaze->myWalls[i] =  myMaze->myWalls[i2];
                myMaze->myWalls[i2] = temp;
            } 

    //parcour all the walls. At each iteration, adjacent cells are tested to be in the same subset of element.
    //If not, they are put in the same subset and the wall between them is opened
    int wallsToTest = 0;//to parcour the walls
    bool close = false;//to tell if a wall is closed
    
    //find index of Cell1 and Cell2 from their coord
    size_t indexCell1, indexCell2, numElem;
    numElem = sqrt(mzSize(myMaze));
    
    while(!mzIsValid(myMaze))
    {
        indexCell1 = myMaze->myWalls[wallsToTest].Cell1.row * numElem + myMaze->myWalls[wallsToTest].Cell1.col;
        indexCell2 = myMaze->myWalls[wallsToTest].Cell2.row * numElem + myMaze->myWalls[wallsToTest].Cell2.col;        
        ufStatus status = (ufStatus)20;//pas sur de cette instruction
                //Merges two cells only if they are not already in the same subset
               status = ufUnion(myMaze->union_Find, indexCell1,indexCell2);
               if (status == UF_MERGED)
               {
                    close = mzIsWallClosed(myMaze, myMaze->myWalls[wallsToTest].Cell1,myMaze->myWalls[wallsToTest].Cell2);
                    mzSetWall(myMaze, myMaze->myWalls[wallsToTest].Cell1,myMaze->myWalls[wallsToTest].Cell2, close);
               }
               wallsToTest++;
    }
    return myMaze;
}

bool mzIsValid(const Maze* maze)
{
    size_t firstElement = ufFind(maze->union_Find, 0);
    for(size_t i = 1; i < maze->number_elements; i++)
    {
        if (firstElement != ufFind(maze->union_Find, i))
            return false;
    }
    return true;
}
bool mzIsWallClosed(Maze* maze, Coord cell1, Coord cell2)
{
   
    for(size_t i=0;i < maze->number_inner_walls; i++)
    {
        if((maze->myWalls[i].Cell1.row == cell1.row && maze->myWalls[i].Cell1.col == cell1.col
         && maze->myWalls[i].Cell2.row == cell2.row && maze->myWalls[i].Cell2.col == cell2.col)
         || (maze->myWalls[i].Cell2.row == cell1.row && maze->myWalls[i].Cell2.col == cell1.col
         && maze->myWalls[i].Cell1.row == cell2.row && maze->myWalls[i].Cell1.col == cell2.col))
        {
            if (maze->myWalls->wall_between == true)
            return false;
            else
            return true;
        }
    }
    return true;
}
void mzSetWall(Maze* maze, Coord cell1, Coord cell2, bool close)
{
    for(size_t i=0;i < maze->number_inner_walls; i++)
    {
        if((maze->myWalls[i].Cell1.row == cell1.row && maze->myWalls[i].Cell1.col == cell1.col
         && maze->myWalls[i].Cell2.row == cell2.row && maze->myWalls[i].Cell2.col == cell2.col)
         || (maze->myWalls[i].Cell2.row == cell1.row && maze->myWalls[i].Cell2.col == cell1.col
         && maze->myWalls[i].Cell1.row == cell2.row && maze->myWalls[i].Cell1.col == cell2.col))
        {
           if (close == true)
            maze->myWalls->wall_between = false;
            else
             maze->myWalls->wall_between = true;
             return;
        }
    }
}

void mzPrint(const Maze* maze, FILE* out)
{
    return;
}

size_t mzSize(const Maze* maze)
{
    return maze->number_elements;
}

void mzFree(Maze* maze)
{
    ufFree(maze->union_Find);
    free(maze->union_Find);
    free(maze->myWalls);
}
