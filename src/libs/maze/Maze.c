#include <stdio.h>
#include <stbool.h>

struct Maze {
    union_find_t *unionFind;
    Walls *myWalls;
    size_t number_elements;
    size_t number_inner_walls;
}

//structure representing two adjacent cells, with a wall or not between them.
typedef struct walls_t Walls;
struct Walls {
    int Cell1;
    int Cell2; 
    bool wall_between;
}
typedef struct maze_t Maze;

Maze* mzCreate(size_t size)
{
    int innerWalls;//le nombre de murs intérieurs.
    struct Maze *myMaze;
    myMaze->number_elements = size*size;
    //set up the struct UnionFind
    myMaze->(*UnionFind) = ufCreate(size*size);

    //setting up the walls of the maze
    //number of inner walls is size*((size-1)*2)
    innerWalls = size*((size-1)*2);
    myMaze->(*myWalls) = malloc(innerwalls*sizeof(walls_t))
    myMaze->number_inner_walls = innerWalls;
    //setting up walls between adjacent cells. Vertical walls are at the beginning of the array.
    //Horizontal walls are a the end of the arrey
    int setVerWalls = 0;
    int setHorWalls = 0;
    for(int i=0;i<(size*size);i++)
        {
            if(i%size < size-1)//set a wall between a cell and it's right neighbour
            {           
            myMaze->myWalls[setVerWalls].Cell1=i;
            myMaze->myWalls[setVerWalls].Cell2=i+1;
            myMaze->myWalls[setVerWalls].wall_between=true;
            setVerWalls ++;
            }
            if(i/size < size-1)//set a wall between a cell and it's bottom neighbour
            {           
            myMaze->myWalls[setHorWalls + innerWalls/2].Cell1=i;
            myMaze->myWalls[setHorWalls + innerWalls/2].Cell2=i+1;
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
    while(!mzIsValid( &myMaze))
    {
        // for(int i=0;i<innerWalls;i++)
        //    { 
                ufStatus status = (ufStatus)20;//pas sur de cette instruction
                //Merges two cells only if they are not already in the same subset
               status = ufUnion(myMaze->(&unionFind), myMaze->myWalls[wallsToTest].Cell1, myMaze->myWalls[wallsToTest].Cell2);
               if (status == UF_MERGED)
               {
                    close = mzIsWallClosed(&myMaze, myMaze->myWalls[wallsToTest].Cell1, myMaze->myWalls[wallsToTest].Cell2);
                    mzSetWall(&myMaze, myMaze->myWalls[wallsToTest].Cell1, myMaze->myWalls[wallsToTest].Cell2, close)
               }
               wallsToTest++;
    }
    return &myMaze;
}

bool mzIsValid(const Maze* maze)
{
    size_t firstElement = ufFind(maze->unionFind, 0);
    for(int i = 1; i < maze->number_elements; i++)
    {
        if (firstelement != ufFind(maze->unionFind, i));
            return false;
    }
    return true;
}
bool mzIsWallClosed(Maze* maze, Coord cell1, Coord cell2)
{
    for(int i=0;i < maze->number_inner_walls; i++)
    {
        if((maze->myWalls[i].Cell1 == cell1 && maze->myWalls[i].Cell2 == cell2) || 
        (maze->myWalls[i].Cell2 == cell1 && maze->myWalls[i].Cell1 == cell2))
        {
            if (maze->myWalls.wall_between == true)
            return false;
            else
            return true;
        }
    }
}
void mzSetWall(Maze* maze, Coord cell1, Coord cell2, bool close)
{
    for(int i=0;i < maze->number_inner_walls; i++)
    {
        if((maze->myWalls[i].Cell1 == cell1 && maze->myWalls[i].Cell2 == cell2) || 
        (maze->myWalls[i].Cell2 == cell1 && maze->myWalls[i].Cell1 == cell2))
        {
           if (close == true)
            maze->myWalls.wall_between = false;
            else
             maze->myWalls.wall_between = true;
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
    ufFree(maze->(&unionFind));
    free(maze->unionFind);
    free(maze->myWalls);
}