#ifndef UTIL_BMP_H

#define UTIL_BMP_H

#include <stdint.h>
#include "list.h"

#define BMP_SIGN 0x4D42

enum bmp_open_result {
  BMP_OK = 0,             // OK
  BMP_FILE_ERR,           // cannot open file
  BMP_HEADER_INV,         // invalid header
  BMP_READ_ERR,           // cannot read from file
  BMP_WR_SIGN,            // file has non-bmp signature
  BMP_DEPTH_UNSPTD,       // pixel depth unsupported
  BMP_MALLOC_ERR,         // failed to malloc
  BMP_NULL_PTR,           // one of args is NULL
};

void bmp_print_err(enum bmp_open_result);

enum bmp_open_result open_bmp(char* path, struct list** list_pp);

struct __attribute__((packed)) bmp_header {
  uint16_t bfType;
  uint32_t bfileSize;
  uint32_t bfReserved;
  uint32_t bOffBits;
  uint32_t biSize;
  uint32_t biWidth;
  uint32_t biHeight;
  uint16_t biPlanes;
  uint16_t biBitCount;
  uint32_t biCompression;
  uint32_t biSizeImage;
  uint32_t biXPelsPerMeter;
  uint32_t biYPelsPerMeter;
  uint32_t biClrUsed;
  uint32_t biClrImportant;
};

#endif  // !UTIL_BMP_H
