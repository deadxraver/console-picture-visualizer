#include "list.h"
#include <malloc.h>
#include <string.h>

/*
  * Initialize list with `sz` elements of size `elem_sz`
  *
  * Sets all elements to zero state
  *
  * Returns NULL if fails to alloc any part
*/
struct list* list_init(size_t sz, size_t elem_sz) {
  struct list* list_p = (struct list*) malloc(sizeof(struct list));
  if (list_p == NULL)
    return NULL;
  list_p->sz = sz;
  list_p->elem_sz = elem_sz;
  list_p->has_inner = false;
  list_p->data = malloc(sz * elem_sz);
  if (list_p->data == NULL) {
    free(list_p);
    return NULL;
  }
  memset(list_p->data, 0, sz * elem_sz);
  return list_p;
}


/*
  * Free list
  *
  * If `has_inner` flag is set, interpret elements from `data`
  * as list pointers, and free recursively them as well
*/
void list_free(struct list* list_p) {
  if (list_p == NULL)
    return;
  if (list_p->has_inner) {
    struct list** list_p_arr = (struct list**) list_p->data;
    for (size_t i = 0; i < list_p->sz; ++i) {
      list_free(list_p_arr[i]);
      list_p_arr[i] = NULL;
    }
    list_p->has_inner = false;
  }
  free(list_p);
}
