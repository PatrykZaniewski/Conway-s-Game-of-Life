#ifndef GENERATION_H_INCLUDED
#define GENERATION_H_INCLUDED

typedef struct {
    int size;
    int elem_in;
    int *positions;
} *array_t;

array_t make_array( int size_given );
array_t make_array_larger( array_t given );
void add_array( array_t given, int position );
void sim_start(int **arr, int amount, int rows, int columns);
void make_changes(int **arr, int *changes, int rows, int columns);
int encode_index(int j, int k, int columns);

#endif
