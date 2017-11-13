#include <stdio.h>

typedef struct {
    int numero;
    struct Sentinel* head;
    struct Element* next;
}Element;
typedef struct {
    struct Element* first;
    struct Element* last;
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
    }
    return unionFind;
}

