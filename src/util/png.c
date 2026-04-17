#include "png.h"

#include "pixel.h"

#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

static const uint8_t PNG_SIGN[] = {137, 80, 78, 71, 13, 10, 26, 10};

static const union { uint32_t type; char type_str[5]; }
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
    case PNG_CRIT_CHUNK_UNSUP: {
      fprintf(stderr, "Criticul chunk met which is currently not supported\n");
      break;
    }
    default: {
      fprintf(stderr, "Unknown error: %d\n", (int)png_open_result);
    }
  }
}

static inline bool is_upper(char c) { return c >= 'A' && c <= 'Z'; }

static enum png_open_result read_chunk(int fd, struct png_chunk* chunk) {
  uint32_t len;
  chunk->data = NULL;
  enum png_open_result res = PNG_OK;

  if (read(fd, &len, sizeof(len)) < sizeof(len)) {
    res = PNG_READ_ERR;
    goto end;
  }

  len = ntohl(len);
  chunk->length = len;

  if (read(fd, &chunk->type.num, sizeof(uint32_t)) < sizeof(uint32_t)) {
    res = PNG_READ_ERR;
    goto end;
  }

  chunk->type.str[4] = 0;
  chunk->data = malloc(len);

  if (chunk->data == NULL) {
    res = PNG_MALLOC_ERR;
    goto end;
  }

  if (read(fd, chunk->data, len) < len) {
    res = PNG_READ_ERR;
    goto end;
  }

  if (read(fd, &chunk->crc, sizeof(uint32_t)) < sizeof(uint32_t)) {
    res = PNG_READ_ERR;
    goto end;
  }

end:
  if (res != PNG_OK && chunk->data != NULL) {
    free(chunk->data);
    chunk->data = NULL;
  }
  return res;
}

enum png_open_result open_png(char* path, struct list** list_pp) {
  enum png_open_result res = PNG_OK;
  struct png_header header;
  struct png_chunk chunk;
  struct list* list = NULL;
  uint8_t sign[8];
  int fd = -1;
  chunk.data = NULL;

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
    if (sign[i] != PNG_SIGN[i]) {
      res = PNG_WR_SIGN;
      goto end;
    }
  }

  res = read_chunk(fd, &chunk);
  if (res != PNG_OK)
    goto end;
  uint32_t length = chunk.length;
  if (length != sizeof(struct png_header)
    || chunk.type.num != hdr_c.type) {
    res = PNG_HDR_ERR;
    goto end;
  }

  memcpy(&header, chunk.data, sizeof(header));
  free(chunk.data);
  chunk.data = NULL;

  uint32_t width = ntohl(header.width), height = ntohl(header.height);

  printf("%s: %ux%u\n"
         "depth: %d, color_type: %d, "
         "compression_type: %d, filtration_type: %d,\n"
         "interlace: %d\n",
         path, (int)width, (int)height,
         (int)header.bit_depth, (int)header.color_type,
         (int)header.compression_type, (int)header.filtration_type,
         (int)header.interlace);

  *list_pp = list_init(height, sizeof(struct list*));
  list = *list_pp;

  if (list == NULL) {
    res = PNG_MALLOC_ERR;
    goto end;
  }

  list->has_inner = true;
  struct list** rows = (struct list**)list->data;
  for (size_t i = 0; i < list->sz; ++i) {
    rows[i] = list_init(width, sizeof(struct pixel));
    if (rows[i] == NULL) {
      res = PNG_MALLOC_ERR;
      goto end;
    }
  }

  while(1) {
    res = read_chunk(fd, &chunk);
    if (res != PNG_OK)
      goto end;
    printf("%s", chunk.type.str);
    if (is_upper(chunk.type.str[0])) {
      printf(": critical");
      // we'll skip non critical chunks
    }
    printf("\n");
    if (chunk.data) {
      free(chunk.data);
      chunk.data = NULL;
    }
    if (chunk.type.num == end_c.type)
      break;
  }

  res = PNG_NO_IMPL;
end:
  if (fd >= 0) {
    close(fd);
    fd = -1;
  }
  if (chunk.data != NULL) {
    free(chunk.data);
    chunk.data = NULL;
  }
  if (res != PNG_OK && list != NULL) {
    list_free(list);
    list = NULL;
  }
  return res;
}
