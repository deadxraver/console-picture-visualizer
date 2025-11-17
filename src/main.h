#ifndef MAIN_H

#define MAIN_H

#define DEFAULT_WIDTH   100

#define MACRO_PRINT_PIXEL(r, g, b)  printf("\e[48;2;%d;%d;%dm%s\e[0m", r, g, b, "  ")

#endif // !MAIN_H
