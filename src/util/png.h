#ifndef UTIL_PNG_H

#define UTIL_PNG_H

#include <stdint.h>
#include "list.h"

enum png_open_result {
  PNG_OK = 0,             // OK
  PNG_FILE_ERR,           // cannot open file
  PNG_READ_ERR,           // cannot read from file
  PNG_WR_SIGN,            // file has non-bmp signature
  PNG_HDR_ERR,            // smth wrong with header
  PNG_MALLOC_ERR,         // failed to malloc
  PNG_NULL_PTR,           // one of args is NULL
  PNG_NO_IMPL,            // format not supported yet
};

enum png_open_result open_png(char* path, struct list** list_pp);

void png_print_err(enum png_open_result);

struct __attribute__((packed)) png_chunk {
  uint32_t length;
  union {
    uint32_t num;
    char str[4];
  } type;
};

struct __attribute__((packed)) png_header {
  uint32_t width;
  uint32_t height;
  uint8_t bit_depth;
  uint8_t color_type;
  uint8_t compression_type;
  uint8_t filtration_type;
  uint8_t interlace;
};

#endif  // !UTIL_PNG_H
