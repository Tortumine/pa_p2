#include <stdio.h>
#include <stdlib.h>
#include "UnionFind.h"

struct union_find_t {
    size_t* items;
    size_t* parrents;
    size_t* rank;
    size_t n_items;
    size_t n_trees;
};

/* ------------------------------------------------------------------------- *
 * Build a union find structure for $n_items$ items (initially each in their
 * respective component).
 *
 * PARAMETERS
 * n_items      Number of items to store in the union find structure
 *
 * NOTE
 * The returned structure should be cleaned with $freeUnionFind$ after
 * usage.
 *
 * RETURN
 * union_find   The created union find structure.
 * ------------------------------------------------------------------------- */
UnionFind* ufCreate(size_t n_items)
{
    // Memory allocation
    UnionFind* tmp = malloc(sizeof(UnionFind));
    if(tmp == NULL)
        return (NULL);
    tmp->items = malloc(n_items * sizeof(size_t));
    if(tmp->items == NULL)
        return (NULL);
    tmp->parrents = malloc(n_items * sizeof(size_t));
    if(tmp->parrents == NULL)
        return (NULL);
    tmp->rank = malloc(n_items * sizeof(size_t));
    if(tmp->rank == NULL)
        return (NULL);
    tmp->n_items = n_items;
    tmp->n_trees = n_items;

    //Inisializing
    for(size_t i = 0; i<n_items; i++)
    {
        tmp->items[i]=i;
        tmp->parrents[i]=i;
        tmp->rank[i]=0;
    }
    return tmp;
}

/* ------------------------------------------------------------------------- *
 * Free the memory allocated for the union find structure.
 *
 * PARAMETERS
 * union_find   The UnionFind structure
 * ------------------------------------------------------------------------- */
void ufFree(UnionFind* union_find)
{
    free(union_find->items);
    free(union_find->parrents);
    free(union_find->rank);
    free(union_find);
}

/* ------------------------------------------------------------------------- *
 * Merge the components containing the items $item1$ and $item2$.
 *
 * PARAMETERS
 * union_find   The UnionFind structure
 * item1        The item in the first component to merge
 * item2        The item in the second component to merge
 *
 * NOTE
 * Both items should be known by the structure.
 *
 * RETURN
 * status       The resulting status of the merge operation; UF_SAME means
 *              the two items were already in the same component
 * ------------------------------------------------------------------------- */
ufStatus ufUnion(UnionFind* union_find, size_t item1, size_t item2)
{
    if(union_find==NULL)return(UF_ERROR);
    if((union_find->n_items <= item1) || (union_find->n_items <= item2))return(UF_ERROR);
    
    size_t root1 = ufFind(union_find,item1);
    size_t root2 = ufFind(union_find,item2);

    if(root1 != root2)
    {
        if(union_find->rank[root1] < union_find->rank[root2])
        {
            union_find->parrents[root1] = root2;
        }
        else
        {
            union_find->parrents[root2] = root1;
            if(union_find->rank[root1] == union_find->rank[root2])
            {
                union_find->rank[root1]++;
            }
        }
        union_find->n_trees--;
        return UF_MERGED;
    }
    return UF_SAME;
}

/* ------------------------------------------------------------------------- *
 * Search for the component of an item.
 *
 * PARAMETERS
 * union_find   The UnionFind structure
 * item         The item for which the component should be searched
 *
 * NOTE
 * The item should be known by the structure
 *
 * RETURN
 * component    the index of the component where $item$ belongs
  * ------------------------------------------------------------------------- */
size_t ufFind(const UnionFind* union_find, size_t item)
{
    if(union_find->parrents[item]!=item)
    {
        union_find->parrents[item]=ufFind(union_find,union_find->parrents[item]);
    }
    return union_find->parrents[item];
}

/* ------------------------------------------------------------------------- *
 * Count the number of components.
 *
 * PARAMETERS
 * union_find   The UnionFind structure
 *
 * RETURN
 * count        The number of components
 * ------------------------------------------------------------------------- */
size_t ufComponentsCount(const UnionFind* union_find)
{
    return union_find->n_trees;
}
