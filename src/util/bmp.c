#include "bmp.h"
#include "pixel.h"
#include <malloc.h>
#include <fcntl.h>
#include <unistd.h>

uint8_t padding_skipper[3];

enum bmp_open_result open_bmp(char* path, struct list** list_pp) {
  if (path == NULL || list_pp == NULL)
    return BMP_NULL_PTR;
  int fd = open(path, O_RDONLY);
  if (fd < 0)
    return BMP_FILE_ERR;
  struct bmp_header* header_p = (struct bmp_header*) malloc(sizeof(struct bmp_header));
  if (header_p == NULL) {
    close(fd);
    fd = -1;
    return BMP_MALLOC_ERR;
  }
  if (read(fd, header_p, sizeof(struct bmp_header)) != sizeof(struct bmp_header)) {
    close(fd);
    fd = -1;
    free(header_p);
    header_p = NULL;
    return BMP_READ_ERR;
  }
  if (header_p->bfType != BMP_SIGN) {
    free(header_p);
    header_p = NULL;
    close(fd);
    fd = -1;
    return BMP_WR_SIGN;
  }
  if (header_p->biBitCount != 24) {
    free(header_p);
    header_p = NULL;
    close(fd);
    fd = -1;
    return BMP_DEPTH_UNSPTD;
  }
  *list_pp = list_init(header_p->biHeight, sizeof(struct list*));
  uint32_t padding = (4 - header_p->biWidth * sizeof(struct pixel) % 4) % 4;
  struct list* list_p = *list_pp;
  list_p->has_inner = true;
  for (size_t i = 0; i < list_p->sz; ++i) {
    ((struct list**)(list_p->data))[i] = list_init(header_p->biWidth, sizeof(struct pixel));
    if (read(fd, ((struct list**)(list_p->data))[i], sizeof(struct pixel) * header_p->biWidth) != sizeof(struct pixel) * header_p->biWidth
        || read(fd, padding_skipper, padding) != padding) {
      free(header_p);
      header_p = NULL;
      close(fd);
      fd = -1;
      list_free(*list_pp);
      *list_pp = NULL;
      list_p = NULL;
      return BMP_READ_ERR;
    }
  }
  free(header_p);
  header_p = NULL;
  close(fd);
  fd = -1;
  return BMP_OK;
}
