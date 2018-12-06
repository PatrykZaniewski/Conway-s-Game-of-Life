#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <png.h>
//#include "print.h"
int x, y;

int width, height;
png_byte color_type;
png_byte bit_depth;

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep * row_pointers;

void make_gif(int amount)
{
    char left [150] = "convert -delay 10 -background white -alpha remove -loop 0 './images/%d.png";
    char right [30];
    sprintf(right,"[0-%d]' generations.gif", amount);
    strcat(left, right);
    system(left);
}

void write_png_file(char* file_name) {

    FILE *fp = fopen(file_name, "wb");
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    info_ptr = png_create_info_struct(png_ptr);
    png_init_io(png_ptr, fp);
    png_set_IHDR(png_ptr, info_ptr, width, height,
                 bit_depth, color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info(png_ptr, info_ptr);
    png_write_image(png_ptr, row_pointers);
    png_write_end(png_ptr, NULL);
    
    for (y=0; y<height; y++)
        free(row_pointers[y]);
    free(row_pointers);
    
    fclose(fp);
}

void print_image(char *fname, int **arr, int rows, int columns) {
    width = columns;
    height = rows;
    bit_depth = 8;
    color_type = PNG_COLOR_TYPE_GRAY;
    number_of_passes = 7;
    row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
    for (y=0; y<height; y++)
        row_pointers[y] = (png_byte*) malloc(sizeof(png_byte) * width);

    for (y=0; y<height; y++) {
        png_byte* row = row_pointers[y];
        for (x=0; x<width; x++) {
            if(arr[y+1][x+1]==0)row[x] = 255;
            else row[x] = 0;
        }
    }
    write_png_file(fname);
}

int print_file(char *fname, int **arr, int rows, int columns)
{

    FILE *ou = fopen (fname, "w");
    if(ou==NULL)
    {
        printf("Nie mozna otworzyc pliku do wypisania\n");
        return 1;
    }
    fprintf(ou, "%d %d\n", rows, columns);
    int i, j;
    for (i=0; i<rows; i++)
    {
        for (j=0; j<columns; j++)
            fprintf(ou, "%d", arr[i+1][j+1]);
        fprintf(ou, "\n");
    }
    for (j=0; j<rows; j++)
    {
        free(arr[j]);
    }
    free(arr);
    return 0;
}
