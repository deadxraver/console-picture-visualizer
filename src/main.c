#include <stdio.h>
#include "main.h"
#include "util/bmp.h"
#include "util/list.h"
#include "util/pixel.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <file>.bmp\n", argv[0]);
    return -1;
  }
  struct list* pixels;
  enum bmp_open_result res = open_bmp(argv[1], &pixels);
  if (res) {
    bmp_print_err(res);
    return res;
  }
  for (size_t i = 0; i < pixels->sz; ++i) {
    struct list* row = ((struct list**)(pixels->data))[i];
    for (size_t j = 0; j < row->sz; ++j) {
      struct pixel pixel = ((struct pixel*)(row->data))[j];
      MACRO_PRINT_PIXEL(pixel.r, pixel.g, pixel.b);
    }
    printf("\n");
  }
  list_free(pixels);
  pixels = NULL;
  return 0;
}
