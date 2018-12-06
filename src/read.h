#ifndef READ_H_INCLUDED
#define READ_H_INCLUDED

int **read_image(char *fname, int *rows, int *columns);
int **read_file(char *fname, int *rows, int *columns);
int **random_image(int*rows, int *columns);

#endif
