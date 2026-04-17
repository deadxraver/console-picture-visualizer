#ifndef UTIL_PNG_H

#define UTIL_PNG_H

#include <stdint.h>
#include "list.h"

enum png_open_result {
  PNG_OK = 0,             // OK
  PNG_FILE_ERR,           // cannot open file
  PNG_READ_ERR,           // cannot read from file
  PNG_WR_SIGN,            // file has non-bmp signature
  PNG_MALLOC_ERR,         // failed to malloc
  PNG_NULL_PTR,           // one of args is NULL
  PNG_NO_IMPL,            // format not supported yet
};

enum png_open_result open_png(char* path, struct list** list_pp);

void png_print_err(enum png_open_result);

struct __attribute__((packed)) png_header {
  uint8_t type[8];
};

#endif  // !UTIL_PNG_H
