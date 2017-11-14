#include <stdio.h>

typedef struct {
    int numero;
    struct Sentinel* head;
    struct Element* next;
}Element;
typedef struct {
    struct Element* first;
    struct Element* last;
    int numberElements;
}Sentinel;
struct union_find_t{
    struct Element* elements;
    struct Sentinel* sentinels;
}UnionFind;

UnionFind* ufCreate(size_t n_items)
{
    UnionFind* unionFind = NULL;
    unionFind = malloc(sizeof(union_find_t));
    unionFind->elements = malloc(n_items * n_items * sizeof(Element));
    unionFind->sentinels = malloc(n_items * n_items * sizeof(Sentinel));
    for(int i=0; i <n_items*n_items; i ++)
    {
        unionFind->elements[i].numero = i;
        unionFind->elements[i].next = NULL;
        unionFind->elements[i].head = &unionFind->sentinels[i];
        unionFind->sentinels[i].first = &unionFind->elements[i];
        unionFind->sentinels[i].last = &unionFind->elements[i];
        unionFind->sentinels[i].numberElements = 1;
    }
    return &unionFind;
}

void ufFree(UnionFind* union_find)
{
    free(unionFind->sentinels);
    free(unionFind->elements);
    free(unionFind);
}

size_t ufFind(const UnionFind* union_find, size_t item)
{
    //takes the head element pointed by the sentinel of the item transmited as argument.
    return union_find->elements[item].head->first->numero;
}

ufStatus ufUnion(UnionFind* union_find, size_t item1, size_t item2)
{
    if (ufFind(&union_find, item1) == ufFind(&union_find, item2))
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
        union_find->elements[toHost].head->last->(*next) = union_find->elements[toConcat].head->(&first);
        union_find->elements[toHost].head->(*last) = union_find->elements[toConcat].head->(&last);
        struct Element* pointToHead;
        struct Sentinel* pointToSentinel;
        pointToHead = union_find->elements[toConcat].head.first;
        pointToSentinel = union_find->elements[toConcat].head;

       
        for(size_t i = 0; i < union_find->elements[toConcat].head.numberElements;i++)
        {
            pointToHead->head = union_find->elements[toHost].head; 
            if (pointToHead->next != null)
                pointToHead = pointToHead->next;
        }
         union_find->elements[toHost].head.numberElements += pointToSentinel->numberElements;
         pointToSentinel->first = NULL;
         pointToSentinel->last = NULL; 

         return UF_MERGED;
    }
}