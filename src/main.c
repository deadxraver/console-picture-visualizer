#include <stdio.h>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Wrong number of args!\n");
    fprintf(stderr, "Usage: %s file.bmp\n", argv[0]);
    return -1;
  }
  return 0;
}
