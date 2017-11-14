#include "UnionFind.h"
#include <stdio.h>

struct union_find_t {
    Item *lab[];
    Item tree_array[];
};

typedef struct item {
    item* parent;
    size_t element;
    size_t rank;
} Item;

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
    UnionFind* union_find_ptr = malloc(sizeof(UnionFind));
    Item *item_array = malloc(n_items * sizeof(Item));
    Item *lab_item_ptr[]= malloc(n_items * sizeof(Item*));

    //Structures initializing
    for(size_t i = 0;i < n_items;i++)
    {   
        item_array[i].element = i;
        item_array[i].parent = NULL;
        item_array[i].rank = 0;

        lab_item_ptr[i] = &item_array[i];
    }
    //return structure ptr copy
    union_find_ptr.lab = lab_item_ptr;
    union_find_ptr.tree_array = lab_item_ptr;
    return union_find_ptr;
}

/* ------------------------------------------------------------------------- *
 * Free the memory allocated for the union find structure.
 *
 * PARAMETERS
 * union_find   The UnionFind structure
 * ------------------------------------------------------------------------- */
void ufFree(UnionFind* union_find)
{
    free(union_find.tree_array);
    free(union_find.lab);
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
    //For maximize the speed , the better way is to 
    //merge on the root of the tree
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
    if(union_find.lab[item].parent == NULL)
    {
        return union_find.lab[item].element;
    }        
    return(ufFindBis(union_find.lab[item].parrent));
}

/* ------------------------------------------------------------------------- *
 * Search for the component of an item.
 *
 * PARAMETERS
 * item_ptr     Pointer to the item for which the component should be searched
 *
 * RETURN
 * component    the index of the component where $item$ belongs
  * ------------------------------------------------------------------------- */
size_t ufFindBis(item* item_ptr)
{
    if(item_ptr.parrent == NULL)
        {
            return item_ptr.element;
        }
    return(ufFindBis(item_ptr.parrent));
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
size_t ufComponentsCount(const UnionFind* union_find);
