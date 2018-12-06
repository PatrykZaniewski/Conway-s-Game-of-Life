#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read.h"
#include "border.h"
#include "generation.h"
#include "print.h"

int main (int argc, char **argv)
{
    int rows = 0, columns = 0, amount;
    int **arr;
    char * file_out;
    if(argc==1)printf("Błąd 03: Brak pliku wejściowego. Aby uzyskać szczegółowe informacje uruchom program z argumentem -man\n");

    if(argc>=2)
    {
        int len = strlen (argv[1]);
        if(argv[1][len-4]=='.' && argv[1][len-3]=='t' && argv[1][len-2]=='x' && argv[1][len-1]=='t')
            arr=read_file(argv[1], &rows, &columns);
        else if(argv[1][len-4]=='.' && argv[1][len-3]=='p' && argv[1][len-2]=='n' && argv[1][len-1]=='g')
            arr=read_image(argv[1], &rows, &columns);
        else if(strcmp(argv[1], "random")==0)arr=random_image(&rows, &columns);
        else if(strcmp(argv[1],"-man")==0 || strcmp(argv[1],"man")==0)
        {
            printf("Program symulujący działanie automatu komórkowego.\n"
                           "Aby poprawnie uruchomić program podaj argumenty w następującej kolejności:\n"
                           "./a.out nazwaplikuwejściowego ilośćgeneracji nazwaplikuwyjściowego\n\n"
                           "Niepodanie drugiego i trzeciego argumentu spowoduje nadaniem im domyślnych wartości, odpowiednio: 16 i 'wyniki.txt\n"
                           "Aby uruchomić program z losowo wygenerowaną planszą zamiast nazwy pliku wpisz 'random'\n");
            return 0;
        }
        else
        {
            printf("Błąd 04: ”Niepoprawny plik wejściowy %s\n", argv[1]);
            return 1;
        }
    }
    else return 1;

    if(argc<3)
    {
        printf("Ostrzeżenie 04: Brak argumentu oznaczającego ilość generacji. Program przyjmuje domyślną wartość 16\n");
        amount=16;
    }
    else
    if(atoi(argv[2])>0)amount=atoi(argv[2]);
    else if (atoi(argv[2])==0)
    {
        printf("Błąd 02: Liczba generacji zawiera niepoprawne znaki. Liczba generacji została ustawiona na domyślną wartość 16\n");
        amount=16;
    }
    else
    {
        printf("Błąd 01: Wczytana liczba generacji jest ujemna. Liczba generacji została ustawiona na domyślną wartość 16\n");
        amount=16;
    }

    if(argc<4)
    {
        printf("Błąd 06: Nie podano nazwy pliku wyjściowego, została nadana domyślna nazwa 'export.txt'\n");
        file_out = "export.txt";
    }
    else
    {
        file_out = argv[3];
    }

    if(arr==NULL)return 1;

    make_border(arr, rows, columns);
    system("rm -rf images");
    system("mkdir images");
    sim_start(arr, amount, rows, columns);
    print_file(file_out, arr, rows, columns);

    return 0;
}
