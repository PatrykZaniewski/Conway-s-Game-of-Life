#include "border.h"

void make_border(int **arr, int rows, int columns)
{
    int j, k;
    for (j=0; j<rows+2; j++)
    {
        arr[j][0]='2'-'0';
        arr[j][columns+1]='2'-'0';
    }
    for (k=0; k<columns+2; k++)
    {
        arr[0][k]='2'-'0';
        arr[rows+1][k]='2'-'0';
    }
}
