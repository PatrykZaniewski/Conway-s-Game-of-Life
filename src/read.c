#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <png.h>
#include "read.h"

#define MAX_READ 1000000

int width, height;
png_byte color_type;
png_byte bit_depth;
png_bytep *row_pointers;

int **read_image(char *fname, int *rows, int *columns)
{
    FILE *in = fopen(fname, "rb");
    if(in==NULL)
    {
        printf("Błąd 04: ”Niepoprawny plik wejściowy %s\n", fname);
        return NULL;
    }
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png) abort();

    png_infop info = png_create_info_struct(png);
    if(!info) abort();

    if(setjmp(png_jmpbuf(png))) abort();

    png_init_io(png, in);

    png_read_info(png, info);

    width      = png_get_image_width(png, info);
    height     = png_get_image_height(png, info);
    color_type = png_get_color_type(png, info);
    bit_depth  = png_get_bit_depth(png, info);
    *rows=height;
    *columns=width;

    *rows+=2;
    *columns+=2;

    if(bit_depth == 16)
        png_set_strip_16(png);

    if(color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if(png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    if(color_type == PNG_COLOR_TYPE_RGB ||
       color_type == PNG_COLOR_TYPE_GRAY ||
       color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if(color_type == PNG_COLOR_TYPE_GRAY ||
       color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    int y;
    for(y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
    }

    png_read_image(png, row_pointers);

    fclose(in);
    int i,j,x;

    int **arr = (int **)malloc((*rows) * sizeof(int *));
    for (i=0; i<*rows; i++)
        arr[i] = (int *)malloc((*columns) * sizeof(int));

    for (i=0; i<*rows; i++)
        for (j=0; j<*columns; j++)
            arr[i][j]=0;

    for(y = 0; y < height; y++) {
        png_bytep row = row_pointers[y];
        for(x = 0; x < width; x++) {
            png_bytep px = &(row[x * 4]);
            if(px[0]==255 && px[1]==255 && px[2]==255 && px[3]==255)arr[y+1][x+1]=0;
            else if(px[0]==0 && px[1]==0 && px[2]==0 && px[3]==255)arr[y+1][x+1]=1;
            else printf("Błąd 05: ”Niepoprawny plik wejściowy %s - błędny symbol w wierszu %d, kolumnie %d\n", fname, y+1, x+1);
        }
    }
    *rows-=2;
    *columns-=2;
    for (y=0; y<height; y++)
        free(row_pointers[y]);
    free(row_pointers);
    return arr;
}


int **read_file(char *fname, int *rows, int *columns)
{
    FILE *in=fopen(fname, "r");
    if(in==NULL)
    {
        printf("Błąd 04: ”Niepoprawny plik wejściowy %s\n", fname);
        return NULL;
    }
    if(fscanf(in, "%d %d\n", rows, columns)!=2)
    {
        printf("Błąd 04: ”Niepoprawny plik wejściowy %s. Dane rozmiaru nie są kompletne.\n", fname);
        return NULL;
    }
    *rows+=2;
    *columns+=2;

    int i, j, k, emptyLine=0, overLoad=0;
    int **arr = (int **)malloc((*rows) * sizeof(int *));
    for (i=0; i<*rows; i++)
         arr[i] = (int *)malloc((*columns) * sizeof(int));

    for (i=0; i<*rows; i++)
        for (j=0; j<*columns; j++)
            arr[i][j]=0;

    int err=0;
        char *buf;
        buf=(char*)malloc(*columns* sizeof(char));
        j=1;
        while(fgets(buf, MAX_READ, in)!=NULL)
        {
            if(j>*rows-2)
            {
                overLoad=1;
                break;
            }
            i=0;
            k=1;
            while(buf[i]!='\0' && buf[i]!='\n')
            {
                    if(buf[i]=='0' || buf[i]=='1')
                    {
                        arr[j][k]=buf[i]-'0';
                        i++;
                        k++;
                    }
                    else
                    {
                            printf("Błąd 05: Niepoprawny plik wejściowy %s - błędny symbol ”%c” w wierszu %d, kolumnie %d\n",fname, buf[i], j, k);
                            err = 1;
                            break;
                    }
                    if(i>*columns-2)
                    {
                        overLoad=1;
                        break;
                    }
            }
            if(i==0 && buf[i]=='\0' && err==0) emptyLine=1;
            if(i<*columns-2 && err==0)emptyLine=1;
            if(err==1)return NULL;
            free(buf);
            buf=(char*)malloc(*columns * sizeof(char));
            j++;
        }
        if(emptyLine==1 || j-1<*rows-2)printf("Ostrzeżenie 01: Podany rozmiar tablicy jest za duży, brakujące kolumny/wiersze zostaną domyslnie wypełnione 0.\n");
        if(overLoad==1)printf("Ostrzeżenie 02: Podany rozmiar tablicy jest za mały, dane wykraczające poza podany rozmiar nie będą zapisane.\n");
    fclose(in);
    free(buf);
    *rows-=2;
    *columns-=2;
    return arr;

}

int **random_image(int *rows, int *columns)
{
    srand(time(NULL));
    *rows = 1 + rand() % 100;
    *columns = 1 + rand() % 100;
    *rows+=2;
    *columns+=2;

    int i, j, k;
    int **arr = (int **)malloc((*rows) * sizeof(int *));
    for (i=0; i<*rows; i++)
        arr[i] = (int *)malloc((*columns) * sizeof(int ));

    for (j=1; j<*rows-1; j++)
        for (k=1; k<*columns-1; k++)
        {
            int value = rand() % 2;
            if(value>=0.5)arr[j][k]=1;
            else arr[j][k]=0;
        }

    *columns-=2;
    *rows-=2;
    return arr;
}
