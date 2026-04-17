#include "png.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

static uint8_t png_sign[] = {137, 80, 78, 71, 13, 10, 26, 10};

void png_print_err(enum png_open_result png_open_result) {
  switch (png_open_result) {
    case PNG_OK: break;
    case PNG_FILE_ERR: {
      fprintf(stderr, "Failed to open file\n");
      break;
    }
    case PNG_READ_ERR: {
      fprintf(stderr, "Failed to read from file\n");
      break;
    }
    case PNG_WR_SIGN: {
      fprintf(stderr, "File is not PNG\n");
      break;
    }
    case PNG_MALLOC_ERR: {
      fprintf(stderr, "Failed to allocate memory\n");
      break;
    }
    case PNG_NULL_PTR: {
      fprintf(stderr, "One of passed args is NULL\n");
      break;
    }
    case PNG_NO_IMPL: {
      fprintf(stderr, "PNG is not implemented yet\n");
      break;
    }
    default: {
      fprintf(stderr, "Unknown error: %d\n", (int)png_open_result);
    }
  }
}

enum png_open_result open_png(char* path, struct list** list_pp) {
  enum png_open_result res = PNG_OK;
  struct png_header header;
  int fd = -1;
  if (path == NULL || list_pp == NULL) {
    res = PNG_NULL_PTR;
    goto end;
  }
  fd = open(path, O_RDONLY);
  if (fd < 0) {
    res = PNG_FILE_ERR;
    goto end;
  }
  if (read(fd, &header, sizeof(struct png_header)) < sizeof(struct png_header)) {
    res = PNG_READ_ERR;
    goto end;
  }
  for (size_t i = 0; i < 8; ++i) {
    if (header.type[i] != png_sign[i]) {
      res = PNG_WR_SIGN;
      goto end;
    }
  }
  res = PNG_NO_IMPL;
  // TODO:
end:
  if (fd >= 0) {
    close(fd);
    fd = -1;
  }
  return res;
}
