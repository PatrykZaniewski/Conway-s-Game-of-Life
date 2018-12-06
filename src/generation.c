#include <stdio.h>
#include <stdlib.h>
#include "print.h"

typedef struct {
    int size;
    int elem_in;
    int *positions;
} *array_t;

array_t make_array( int size_given ) {
    array_t box = malloc( sizeof *box );
    box->positions = malloc( size_given * sizeof( double ) );
    box->size = size_given;
    box->elem_in = 0;
    return box;
}

array_t make_array_larger( array_t given ) {
    given->positions = realloc( given->positions, 2*given->size*sizeof( double ) );
    given->size *= 2;
    return given;
}

void add_array( array_t given, int position ) {
    if( given->size == given->elem_in )
        make_array_larger( given );
    given->positions[ given->elem_in ] = position;
    given->elem_in++;
}

int encode_index (int j, int k, int columns)
{
    return (j - 1) * columns + k;
}

void make_changes(int **arr, array_t changes, int rows, int columns)
{
    int j;
    for( j=0; j<changes->elem_in; j++ )
        {
        int row_change = (changes->positions[j]-1)/columns+1;
        int column_change = (changes->positions[j])%columns;
        if (column_change == 0) column_change = columns;
        if(arr[row_change][column_change]==0)arr[row_change][column_change]=1;
        else arr[row_change][column_change]=0;
        }
        changes->elem_in=0;
}

int sim_start(int **arr, int amount, int rows, int columns)
{

    int count_one=0;
    array_t changes = make_array(100);
    int i, j, k;
    print_image("./images/0.png", arr, rows ,columns);

    for (i=0; i<amount; i++)
    {
        for(j=1; j<=rows; j++)
        {
            for(k=1; k<=columns; k++)
            {
                if(arr[j-1][k]==1)count_one++;
                if(arr[j-1][k+1]==1)count_one++;
                if(arr[j][k+1]==1)count_one++;
                if(arr[j+1][k+1]==1)count_one++;
                if(arr[j+1][k]==1)count_one++;
                if(arr[j+1][k-1]==1)count_one++;
                if(arr[j][k-1]==1)count_one++;
                if(arr[j-1][k-1]==1)count_one++;

                if(arr[j][k]==0)
                {
                    if(count_one==3) {
                        int position = encode_index(j, k, columns);
                        add_array(changes, position);
                    }
                }
                else
                {
                    if(count_one!=2 && count_one!=3) {
                        int position = encode_index(j, k, columns);
                        add_array(changes, position);
                    }
                }
                count_one=0;
            }
            count_one=0;
        }
        make_changes(arr, changes, rows, columns);
        char numbers[20];
        sprintf(numbers, "./images/%d.png", i+1);
        print_image(numbers, arr, rows ,columns);
    }
    make_gif(amount);
    free(changes->positions);
    free(changes);
    return 0;
}


