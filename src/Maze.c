#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "Maze.h"
#include "UnionFind.h"

struct maze_t {
    UnionFind *union_Find;
    struct Walls *myWalls;
    size_t size;
    size_t number_inner_walls;
    size_t number_open_walls;
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
    size_t innerWalls;//le nombre de murs intérieurs.
    Maze *myMaze;
    myMaze = malloc(sizeof(Maze));
    myMaze->size = size;
    //set up the struct UnionFind
    myMaze->union_Find = ufCreate(size*size);
    //setting up the walls of the maze
    //number of inner walls is size*((size-1)*2)
    innerWalls = size*((size-1)*2);
    myMaze->myWalls = malloc(innerWalls*sizeof(*myMaze->myWalls));
    myMaze->number_inner_walls = innerWalls;
    myMaze->number_open_walls = 0;
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
     //The order in which myWalls will be parcoured is determined by the array parcours.
     //Which is an array of shuffle indexes
       size_t* parcours;
       parcours = malloc(innerWalls * sizeof(size_t));
       for (size_t i=0;i<innerWalls;i++)
       {parcours[i] = i;}
        size_t temp;
       for(size_t i=0;i<innerWalls;i++)
            { 
                size_t i2 = rand()%innerWalls;               
                temp =  parcours[i];
                parcours[i] =  parcours[i2];
                parcours[i2] = temp;
            } 
    //parcour all the walls. At each iteration, adjacent cells are tested to be in the same subset of element.
    //If not, they are put in the same subset and the wall between them is opened
    size_t wallsToTest = 0;//to parcour the walls
    bool close;//to tell if a wall is closed
    
    //find index of Cell1 and Cell2 from their coord
    size_t indexCell1, indexCell2;
    size_t visit = 0; //is the index of the wall to visit

    while(!mzIsValid(myMaze) && wallsToTest < innerWalls)
    {     
        visit = parcours[wallsToTest];
        indexCell1 = myMaze->myWalls[visit].Cell1.row * size + myMaze->myWalls[visit].Cell1.col;
        indexCell2 = myMaze->myWalls[visit].Cell2.row * size + myMaze->myWalls[visit].Cell2.col;             
        //Merges two cells only if they are not already in the same subset
        close = mzIsWallClosed(myMaze, myMaze->myWalls[visit].Cell1,myMaze->myWalls[visit].Cell2);
        if ((close == false) && (ufFind(myMaze->union_Find, indexCell1) != ufFind(myMaze->union_Find, indexCell2)))
        {
            mzSetWall(myMaze, myMaze->myWalls[visit].Cell1,myMaze->myWalls[visit].Cell2, !close);
            ufUnion(myMaze->union_Find, indexCell1,indexCell2);
        }
        wallsToTest++;
    }
    free(parcours);
    return myMaze;
}

bool mzIsValid(const Maze* maze)
{
    //if the numbre of subsets in unionfind is > 1 or 
    //the number of open walls is < size^2 -1 return false
    if ((ufComponentsCount(maze->union_Find) > 1) ||
    (maze->number_open_walls != (maze->size * maze->size - 1)))
        return false;
    else return true;
}
bool mzIsWallClosed(const Maze* maze, Coord cell1, Coord cell2)
{
   size_t indexCell1 = cell1.row * maze->size + cell1.col;
   size_t indexCell2 = cell2.row * maze->size + cell2.col;
   size_t indexArray;

   if (indexCell1 == (indexCell2 - 1))
   {
        indexArray = indexCell1 - indexCell1/maze->size;
   } 
   else
    indexArray = indexCell1 + maze->number_inner_walls/2;
   if (maze->myWalls[indexArray].wall_between == true)
        return false;
    else
        return true;
}
void mzSetWall(Maze* maze, Coord cell1, Coord cell2, bool close)
{
   size_t indexCell1 = cell1.row * maze->size + cell1.col;
   size_t indexCell2 = cell2.row * maze->size + cell2.col;
   size_t indexArray;
   if (indexCell1 == (indexCell2 - 1))
   {
        indexArray = indexCell1 - indexCell1/maze->size;
   } 
   else
    indexArray = indexCell1 + maze->number_inner_walls/2;
    if (close == true)
    {
        maze->myWalls[indexArray].wall_between = false;
        maze->number_open_walls++;
    }
        
    else
        maze->myWalls[indexArray].wall_between = true;

}

void mzPrint(const Maze* maze, FILE* out)
{

    /* Pour commencer la ligne du haut est dessinée
     * Cette fonction parcourt le labyrinthe et regarde si y a un mur avec une case voisine
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
     *  Pour finir la ligne du tat est dessinée
     * /!\  Problème, lors des comparaison des cases le résultat renvoyé est toujours "pas de murs"
     * CF mzIsWallClosed(maze,cell_a,cell_b)
     */
    //pFile2 = fopen("myfile2.txt", "a");

    size_t i = 0;
    size_t j = 0;
    char h_open[]   = "+  ";
    char h_close[]  = "+--";

    Coord cell_a,cell_b;


    //first border line
    for(i = 0;i<maze->size;i++)
    {
        fprintf(out,"%s", h_close);
    }
    fprintf(out,"+\n");
    //internals
    //first line (open on left)
    cell_a.col=0;
    cell_a.row=0;
    cell_b.col=1;
    cell_b.row=0;

    for(i=0;i<maze->size;i++) {
        //Vertical
        if (i == 0 && j == 0)//enter
        {
            fprintf(out, " ");
        } else//extreme_Left_wall
        {
            fprintf(out, "|");
        }

        for (j = 0; j < maze->size - 1; j++) {
            cell_a.col = j;
            cell_b.col = j + 1;
            cell_a.row = i;
            cell_b.row = i;
            bool tmp = mzIsWallClosed((Maze*)maze, cell_a, cell_b);
            if (!tmp)
                fprintf(out, "  |");
            else
                fprintf(out, "   ");
        }

        if (i < (maze->size - 1))//extreme_Right_wall
        {
            fprintf(out, "  |\n");
        } else//exit
        {
            fprintf(out, "   \n");
        }

        //switching from vertical walls to horizontals
        cell_a.col = 0;
        cell_b.col = 0;
        cell_a.row = i;
        cell_b.row = cell_a.row + 1;


        //Horizontal walls
        if (i < maze->size - 1) {
            for (j = 0; j < maze->size; j++) {
                cell_a.col = j;
                cell_b.col = j;
                bool tmp = mzIsWallClosed((Maze*)maze, cell_a, cell_b);
                if (!tmp)
                    fprintf(out,"%s", h_close);
                else
                    fprintf(out,"%s", h_open);
            }
            fprintf(out,"+\n");
        } else    //last border line
        {
            for (j = 0; j < maze->size; j++) {
                fprintf(out, "%s", h_close);
            }
            fprintf(out, "+");//pas de retour à la ligne à la fin
        }
    }
}

size_t mzSize(const Maze* maze)
{
    return maze->size;
}

void mzFree(Maze* maze)
{
    ufFree(maze->union_Find);
    free(maze->myWalls);
    free(maze);
}
