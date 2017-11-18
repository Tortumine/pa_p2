#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "Maze.h"
#include "UnionFind.h"

struct maze_t {
    UnionFind *union_Find;
    struct Walls *myWalls;
    size_t size;
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
    myMaze->size = size;
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
            myMaze->myWalls[setVerWalls].Cell2.row=i/size;
            myMaze->myWalls[setVerWalls].Cell2.col=(i%size)+1;
            myMaze->myWalls[setVerWalls].wall_between=true;
            setVerWalls ++;
            }
            if(i/size < size-1)//set a wall between a cell and it's bottom neighbour
            {           
            myMaze->myWalls[setHorWalls + innerWalls/2].Cell1.row=i/size;
            myMaze->myWalls[setHorWalls + innerWalls/2].Cell1.col=i%size;
            myMaze->myWalls[setHorWalls + innerWalls/2].Cell2.row=(i/size)+1;
            myMaze->myWalls[setHorWalls + innerWalls/2].Cell2.col=i%size;
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
    size_t indexCell1, indexCell2;
    
    while(!mzIsValid(myMaze) && wallsToTest < innerWalls)
    {
        indexCell1 = myMaze->myWalls[wallsToTest].Cell1.row * size + myMaze->myWalls[wallsToTest].Cell1.col;
        indexCell2 = myMaze->myWalls[wallsToTest].Cell2.row * size + myMaze->myWalls[wallsToTest].Cell2.col;        
        ufStatus status = (ufStatus)20;//pas sur de cette instruction
        //Merges two cells only if they are not already in the same subset
        status = ufUnion(myMaze->union_Find, indexCell1,indexCell2);
        if (status == UF_MERGED)
        {
            //if cells have been merged in the same subset, the wall between them is open
            close = mzIsWallClosed(myMaze, myMaze->myWalls[wallsToTest].Cell1,myMaze->myWalls[wallsToTest].Cell2);
            mzSetWall(myMaze, myMaze->myWalls[wallsToTest].Cell1,myMaze->myWalls[wallsToTest].Cell2, close);            
        }
        wallsToTest++;
    }
    return myMaze;
}

bool mzIsValid(const Maze* maze)
{
    if (ufComponentsCount(maze->union_Find) > 1)
        return false;
    else return true;
}
bool mzIsWallClosed(Maze* maze, Coord cell1, Coord cell2)
{
   //parcour the array until it finds the wall between the two given cells.
   //returns true if there is no wall set up
   //returns false otherwise
   //complexity O(N)
    for(size_t i=0;i < maze->number_inner_walls; i++)
    {
        if((maze->myWalls[i].Cell1.row == cell1.row && maze->myWalls[i].Cell1.col == cell1.col
         && maze->myWalls[i].Cell2.row == cell2.row && maze->myWalls[i].Cell2.col == cell2.col)
         || (maze->myWalls[i].Cell2.row == cell1.row && maze->myWalls[i].Cell2.col == cell1.col
         && maze->myWalls[i].Cell1.row == cell2.row && maze->myWalls[i].Cell1.col == cell2.col))
        {
            if (maze->myWalls[i].wall_between == true)
            return false;
            else
            return true;
        }
    }
    return true;
}
void mzSetWall(Maze* maze, Coord cell1, Coord cell2, bool close)
{
    //sets the wall between two cells. Parcour of the array until the specific wall is found.
    for(size_t i=0;i < maze->number_inner_walls; i++)
    {
        if((maze->myWalls[i].Cell1.row == cell1.row && maze->myWalls[i].Cell1.col == cell1.col
         && maze->myWalls[i].Cell2.row == cell2.row && maze->myWalls[i].Cell2.col == cell2.col)
         || (maze->myWalls[i].Cell2.row == cell1.row && maze->myWalls[i].Cell2.col == cell1.col
         && maze->myWalls[i].Cell1.row == cell2.row && maze->myWalls[i].Cell1.col == cell2.col))
        {
            if (close == false)
                maze->myWalls[i].wall_between = false;
            else
                maze->myWalls[i].wall_between = true;
            return;
        }
    }
}

void mzPrint(const Maze* maze, FILE* out)
{

    /*
     * Cette fonction parcourt le labirinthe et regarde si y a un mur avec une case voisine
     *   V :D'abord la case à droite col b = col a +1 / row b = row a
     *      ca décale à droite
     *      min col a = 0
     *      max col a = maze->size-2
     *
     *   H :Ensuite la case du dessous col b = col a / row b = row a+1
     *      ca décale à droite
     *      min col a = 0
     *      max col a = maze->size-1
     *
     * /!\  Problème, lors des comparaison des cases le résultat renvoyé est toujours "pas de murs"
     * CF mzIsWallClosed(maze,cell_a,cell_b)
     */
    //pFile2 = fopen("myfile2.txt", "a");

    size_t i = 0;
    size_t j = 0;
    char h_open[]   = "+   ";
    char h_close[]  = "+---";

    Coord cell_a,cell_b;


    //first border line
    for(i = 0;i<maze->size;i++)
    {
        printf( "%s", h_close);
    }
    printf( "+\n");
    //internals
    //first line (open on left)
    cell_a.col=0;
    cell_a.row=0;
    cell_b.col=1;
    cell_b.row=0;

    for(i=0;i<maze->size;i++)
    {
        //Vertical
            if(i==0 && j == 0)//enter
            {
                printf( " ");
            }
            else//extreme_Left_wall
            {
                printf( "|");
            }

            for(j=0;j<maze->size-1;j++)
            {
                cell_a.col=j;
                cell_b.col=j+1;
                cell_a.row=i;
                cell_b.row=i;
                bool tmp = mzIsWallClosed(maze,cell_a,cell_b);
                if(tmp)
                    printf( "   |");
                else
                    printf( "    ");
            }

            if(i<(maze->size-1))//extreme_Right_wall
            {
                printf( "   |\n");
            }
            else//exit
            {
                printf( "    \n");
            }

        //switching from vertical walls to horizontals
        cell_a.col=0;
        cell_b.col=0;
        cell_a.row=i;
        cell_b.row=cell_a.row+1;


        //Horizontal walls
        if(i<maze->size-1) {
            for (j = 0; j < maze->size; j++) {
                cell_a.col = j;
                cell_b.col = j;
                bool tmp = mzIsWallClosed(maze, cell_a, cell_b);
                if (tmp)
                    printf("%s", h_close);
                else
                    printf("%s", h_open);
            }
            printf("+\n");
        }
        else    //last border line
        {
            for(j=0;j<maze->size;j++)
            {
                printf( "%s", h_close);
            }
            printf( "+ \n");
        }
    }
    return;
}

size_t mzSize(const Maze* maze)
{
    return maze->size;
}

void mzFree(Maze* maze)
{
    ufFree(maze->union_Find);
    //free(maze->union_Find);
    free(maze->myWalls);
}
