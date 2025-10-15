#include "compressor.h"
#include "pixel.h"
#include <stdbool.h>

static inline struct list* getlist(struct list* list_p, size_t ind) {
  return ((struct list**)(list_p->data))[ind];
}

static inline struct pixel getpixel(struct list* list_p, size_t ind) {
  return ((struct pixel*)(list_p->data))[ind];
}

static struct pixel merge_pixels(size_t k, size_t i1, size_t j1, struct list* list_p) {
  int r = 0, g = 0, b = 0;
  int counter = 0;
  for (size_t i = i1 * k; i < i1 * (k + 1); ++i) {
    if (i >= list_p->sz)
      continue;
    struct list* row = getlist(list_p, i);
    for (size_t j = j1 * k; j < j1 * (k + 1); ++j) {
      if (j >= row->sz)
        continue;
      ++counter;
      struct pixel pixel = getpixel(row, j);
      r += pixel.r;
      g += pixel.g;
      b += pixel.b;
    }
  }
  if (counter == 0)
    return (struct pixel) {0, 0, 0};
  return (struct pixel) { .r = r / counter, .g = g / counter, .b = b / counter };
}

void compress(struct list** list_pp, size_t width) {
  if (width == 0)
    return;
  size_t k = getlist(*list_pp, 0)->sz / width;
  if (k <= 1)
    return;
  struct list* new_list = list_init((*list_pp)->sz / k, (*list_pp)->elem_sz);
  for (size_t i = 0; i < new_list->sz; ++i) {
    ((struct list**)(new_list->data))[i] = list_init(getlist(*list_pp, 0)->sz / k, getlist(*list_pp, 0)->elem_sz);
  }
  new_list->has_inner = true;
  for (size_t i = 0; i < new_list->sz; ++i) {
    struct list* row = getlist(new_list, i);
    for (size_t j = 0; j < row->sz; ++j) {
      ((struct pixel*)(row->data))[j] = merge_pixels(k, i, j, *list_pp);
    }
  }
  list_free(*list_pp);
  *list_pp = new_list;
  return;
}
