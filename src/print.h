#ifndef PRINT_H_INCLUDED
#define PRINT_H_INCLUDED

void print_image(char *fname, int **arr, int rows, int columns);
void print_file(char *fname, int **arr, int rows, int columns);
void write_png_file(char* file_name);
void make_gif(int amount);

#endif
