#include <stdio.h>
#include "main.h"
#include "util/bmp.h"
#include "util/list.h"
#include "util/pixel.h"
#include "util/compressor.h"

static int parse_uint(char* s) {
  int res = 0;
  for (char* c = s; *c != 0; ++c) {
    if (*c > '9' || *c < '0')
      return -1;
    res *= 10;
    res += *c - '0';
  }
  return res;
}

int main(int argc, char* argv[]) {
  if (argc < 2 || argc == 2 && argv[1][0] == '-' || argc > 3) {
    fprintf(stderr, "Usage: %s [ -<maxwidth> ] <file>.bmp\n", argv[0]);
    return -1;
  }
  char* file = argv[1][0] != '-' ? argv[1] : argv[2];
  int width = argc == 2 ? DEFAULT_WIDTH : file == argv[1] ? parse_uint(argv[2] + 1) : parse_uint(argv[1] + 1);
  if (width < 0) {
    fprintf(stderr, "expected number, got '%s'\n", (file == argv[1] ? argv[2] : argv[1]) + 1);
    return -1;
  }
  struct list* pixels;
  enum bmp_open_result res = open_bmp(file, &pixels);
  if (res) {
    bmp_print_err(res);
    return res;
  }
  compress(&pixels, width);
  for (size_t i = pixels->sz; i > 0; --i) {
    struct list* row = ((struct list**)(pixels->data))[i-1];
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
