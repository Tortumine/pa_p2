#include <stdio.h>
#include <stdlib.h>
#include "UnionFind.h"

struct union_find_t {
    struct Element* elements;
    struct Sentinel* sentinels;
};

typedef struct Element {
    size_t numero;
    struct Sentinel* head;
    struct Element* next;
}Element;
typedef struct Sentinel{
    struct Element* first;
    struct Element* last;
    size_t numberElements;
}Sentinel;



UnionFind * ufCreate(size_t n_items)
{
    UnionFind * unionFind;
    unionFind = malloc(sizeof(UnionFind));
    unionFind->elements = malloc(n_items * n_items * sizeof(*unionFind->elements));
    unionFind->sentinels = malloc(n_items * n_items * sizeof(*unionFind->sentinels));
    for(size_t i=0; i < n_items*n_items; i ++)
    {
        unionFind->elements[i].numero = i;
        unionFind->elements[i].next = NULL;
        unionFind->elements[i].head = &unionFind->sentinels[i];
        unionFind->sentinels[i].first = &unionFind->elements[i];
        unionFind->sentinels[i].last = &unionFind->elements[i];
        unionFind->sentinels[i].numberElements = 1;
    }
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
    //takes the head element pointed by the sentinel of the item transmited as argument.
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
        union_find->elements[toHost].head->last->next = union_find->elements[toConcat].head->first;
        union_find->elements[toHost].head->last = union_find->elements[toConcat].head->last;
        struct Element* pointToHead;
        struct Sentinel* pointToSentinel;
        pointToHead = union_find->elements[toConcat].head->first;
        pointToSentinel = union_find->elements[toConcat].head;
     
        for(size_t i = 0; i < union_find->elements[toConcat].head->numberElements;i++)
        {
            pointToHead->head = union_find->elements[toHost].head; 
            if (pointToHead->next != NULL)
                pointToHead = pointToHead->next;
        }
         union_find->elements[toHost].head->numberElements += pointToSentinel->numberElements;
         pointToSentinel->first = NULL;
         pointToSentinel->last = NULL; 

         return UF_MERGED;
    }
}
size_t ufComponentsCount(const UnionFind* union_find)
{
    size_t j=0;
    return j;
}
