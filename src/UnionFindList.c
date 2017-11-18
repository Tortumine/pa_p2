#include <stdio.h>
#include <stdlib.h>
#include "UnionFind.h"

//structure union find, conntaining an array of elements (cells) 
//and an array of sentinels.
struct union_find_t {
    struct Element* elements;
    struct Sentinel* sentinels;
    size_t numberComponents;
};

//Element is composed of a number and a pointer head to it's sentinel and 
//a pointer to the next element of the subset. Last element of the list "points" to NULL
typedef struct Element {
    size_t numero;
    struct Sentinel* head;
    struct Element* next;
}Element;
//points to the first and last elements of a subset of elements
//contains the number of elements in the subset
typedef struct Sentinel{
    struct Element* first;
    struct Element* last;
    size_t numberElements;
}Sentinel;



UnionFind * ufCreate(size_t n_items)
{
    UnionFind * unionFind;
    unionFind = malloc(sizeof(UnionFind));
    unionFind->elements = malloc(n_items * sizeof(*unionFind->elements));
    unionFind->sentinels = malloc(n_items * sizeof(*unionFind->sentinels));
    
    //At first N subsets of 1 elements and 1 sentinel are created. 
    //Corresponding elements and sentinels share the same array index.
    for(size_t i=0; i < n_items; i ++)
    {
        unionFind->elements[i].numero = i;
        unionFind->elements[i].next = NULL;
        unionFind->elements[i].head = &unionFind->sentinels[i];
        unionFind->sentinels[i].first = &unionFind->elements[i];
        unionFind->sentinels[i].last = &unionFind->elements[i];
        unionFind->sentinels[i].numberElements = 1;      
    }
    unionFind->numberComponents = n_items;
    return unionFind;
}

void ufFree(UnionFind* union_find)
{
    free(union_find->sentinels);
    free(union_find->elements);
    free(union_find);
}

size_t ufFind(const UnionFind* union_find, size_t item)
{
    //takes the numero of the head element pointed by the sentinel of the item transmited as argument.
    return union_find->elements[item].head->first->numero;
}

ufStatus ufUnion(UnionFind* union_find, size_t item1, size_t item2)
{   
    if (ufFind(union_find, item1) == ufFind(union_find, item2))
        return UF_SAME;
    else 
    {
        size_t toConcat, toHost;
        //the subset containing the least elements is merged to the other subset.
       if (union_find->elements[item1].head->numberElements <= union_find->elements[item2].head->numberElements)
            {toConcat = item1;
            toHost = item2;}
        else
            {toConcat = item2;
            toHost = item1;}
        
        //concatenate the two lists    
        union_find->elements[toHost].head->last->next = union_find->elements[toConcat].head->first;
        union_find->elements[toHost].head->last = union_find->elements[toConcat].head->last;
        struct Element* pointToHead;//first element of the subset to concat
        struct Sentinel* pointToSentinel;//sentinel of the subset to concat
        pointToHead = union_find->elements[toConcat].head->first;
        pointToSentinel = union_find->elements[toConcat].head;
        
        //parcour all the elements of the smaller subset to point to the new sentinel
        for(size_t i = 0; i < union_find->elements[toConcat].head->numberElements;i++)
        {
            pointToHead->head = union_find->elements[toHost].head; 
            if (pointToHead->next != NULL)
                pointToHead = pointToHead->next;
        }

        union_find->elements[toHost].head->numberElements += pointToSentinel->numberElements;
        pointToSentinel->numberElements = 0;
        pointToSentinel->first = NULL;
        pointToSentinel->last = NULL; 

        union_find->numberComponents -= 1;
         return UF_MERGED;
    }
}
size_t ufComponentsCount(const UnionFind* union_find)
{
    return union_find->numberComponents;
}
