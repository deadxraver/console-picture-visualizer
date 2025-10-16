#include "compressor.h"
#include "pixel.h"
#include <stdbool.h>
#include <stdlib.h>

uint8_t* r_arr;
uint8_t* g_arr;
uint8_t* b_arr;

static inline struct list* getlist(const struct list* list_p, size_t ind) {
  return ((struct list**)(list_p->data))[ind];
}

static inline struct pixel getpixel(const struct list* list_p, size_t ind) {
  return ((struct pixel*)(list_p->data))[ind];
}

static int comparator(const void* a, const void* b) {
  return (*(const uint8_t*)a - *(const uint8_t*)b);
}

static struct pixel merge_pixels(size_t k, size_t i1, size_t j1, const struct list* list_p) {
  int r = 0, g = 0, b = 0;
  int counter = 0;
  for (size_t i = i1 * k; i < (i1 + 1) * k; ++i) {
    if (i >= list_p->sz)
      continue;
    struct list* row = getlist(list_p, i);
    for (size_t j = j1 * k; j < (j1 + 1) * k; ++j) {
      if (j >= row->sz)
        continue;
      struct pixel pixel = getpixel(row, j);
      r += pixel.r;
      g += pixel.g;
      b += pixel.b;
      ++counter;
    }
  }
  if (counter == 0)
    return (struct pixel) {0, 0, 0};
  return (struct pixel) { .r = r / counter, .g = g / counter, .b = b / counter };
}

static struct pixel median_merge_pixels(size_t k, size_t i1, size_t j1, const struct list* list_p) {
  int r = 0, g = 0, b = 0;
  int counter = 0;
  for (size_t i = i1 * k; i < (i1 + 1) * k; ++i) {
    if (i >= list_p->sz)
      continue;
    struct list* row = getlist(list_p, i);
    for (size_t j = j1 * k; j < (j1 + 1) * k; ++j) {
      if (j >= row->sz)
        continue;
      struct pixel pixel = getpixel(row, j);
      r_arr[counter] = pixel.r;
      g_arr[counter] = pixel.g;
      b_arr[counter] = pixel.b;
      ++counter;
    }
  }
  if (counter == 0)
    return (struct pixel) {0, 0, 0};
  qsort(r_arr, counter, sizeof(uint8_t), comparator);
  qsort(g_arr, counter, sizeof(uint8_t), comparator);
  qsort(b_arr, counter, sizeof(uint8_t), comparator);
  r = r_arr[counter / 2 - 1];
  g = g_arr[counter / 2 - 1];
  b = b_arr[counter / 2 - 1];
  if (counter % 2) {
    r = (r + r_arr[counter / 2]) / 2;
    g = (g + g_arr[counter / 2]) / 2;
    b = (b + b_arr[counter / 2]) / 2;
  }
  return (struct pixel) { .r = r, .g = g, .b = b };
}

void compress(struct list** list_pp, size_t width) {
  if (width == 0 || list_pp == NULL || *list_pp == NULL)
    return;
  const struct list* list_p = *list_pp;
  size_t k = getlist(list_p, 0)->sz / width;
  if (k <= 1)
    return;
  struct list* new_list = list_init(list_p->sz / k, list_p->elem_sz);
  for (size_t i = 0; i < new_list->sz; ++i) {
    ((struct list**)(new_list->data))[i] = list_init(getlist(list_p, 0)->sz / k, getlist(list_p, 0)->elem_sz);
  }
  new_list->has_inner = true;
  r_arr = (uint8_t*) malloc(sizeof(uint8_t) * k * k);
  g_arr = (uint8_t*) malloc(sizeof(uint8_t) * k * k);
  b_arr = (uint8_t*) malloc(sizeof(uint8_t) * k * k);
  for (size_t i = 0; i < new_list->sz; ++i) {
    struct list* row = getlist(new_list, i);
    for (size_t j = 0; j < row->sz; ++j) {
      ((struct pixel*)(row->data))[j] = median_merge_pixels(k, i, j, list_p);
    }
  }
  free(r_arr);
  free(g_arr);
  free(b_arr);
  list_free(*list_pp);
  *list_pp = new_list;
  return;
}
