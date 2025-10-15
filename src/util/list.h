#ifndef UTIL_LIST_H

#define UTIL_LIST_H

#include <stdbool.h>
#include <stddef.h>

struct list {
  size_t sz;
  size_t elem_sz;
  bool has_inner;
  void* data;
};

struct list* list_init(size_t sz, size_t elem_sz);
void list_free(struct list* list_p);

#endif // !UTIL_LIST_H
