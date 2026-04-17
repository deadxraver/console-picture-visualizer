#include "png.h"

#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

static const uint8_t png_sign[] = {137, 80, 78, 71, 13, 10, 26, 10};

static const union { uint32_t type; char type_str[4]; }
  hdr_c = { .type_str = "IHDR" },
  plte_c = { .type_str = "PLTE" },
  data_c = { .type_str = "IDAT" },
  end_c = { .type_str = "IEND" };

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
  struct png_chunk chunk;
  uint32_t crc;
  char type[5] = {0};
  uint8_t sign[8];
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
  if (read(fd, sign, sizeof(sign)) < sizeof(sign)) {
    res = PNG_READ_ERR;
    goto end;
  }
  for (size_t i = 0; i < 8; ++i) {
    if (sign[i] != png_sign[i]) {
      res = PNG_WR_SIGN;
      goto end;
    }
  }
  if (read(fd, &chunk, sizeof(chunk)) < sizeof(chunk)) {
    res = PNG_READ_ERR;
    goto end;
  }
  uint32_t length = ntohl(chunk.length);
  if (length != sizeof(struct png_header)
    || chunk.type.num != hdr_c.type) {
    res = PNG_HDR_ERR;
    goto end;
  }
  if (read(fd, &header, length) < length) {
    res = PNG_READ_ERR;
    goto end;
  }
  if (read(fd, &crc, sizeof(crc)) < sizeof(crc)) {
    res = PNG_READ_ERR;
    goto end;
  }
  uint32_t width = ntohl(header.width), height = ntohl(header.height);
  printf("%s: %ux%u\n"
         "depth: %d, color_type: %d, "
         "compression_type: %d, filtration_type: %d,\n"
         "interlace: %d\n",
         path, (int)width, (int)height,
         (int)header.bit_depth, (int)header.color_type,
         (int)header.compression_type, (int)header.filtration_type,
         (int)header.interlace);
  res = PNG_NO_IMPL;
  // TODO:
end:
  if (fd >= 0) {
    close(fd);
    fd = -1;
  }
  return res;
}
