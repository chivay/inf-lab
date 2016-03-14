/**
 * "Mniej niż dwa"
 *
 * Program przykładowy do zajęć laboratoryjnych ze Wstępu do programowania.
 *
 * Czyta tekst i pisze wiersz z permutacją jego kolumn, w wyniku której
 * kolumny z mniej niż dwoma znakami różnymi od spacji znajdą się na początku.
 *
 * Wynikowa permutacja to ciąg numerów kolumn, liczonych od 0, które mają
 * się znaleźć na danej pozycji. Np. permutacja "2 0 1" mówi, że pierwsza
 * na wyjściu ma być kolumna, która w danych miała numer 2, druga kolumna 0
 * a trzecia kolumna 1.
 *
 * autor: Artur Zaroda <zaroda@mimuw.edu.pl>
 * wersja: 1.0
 * data: 14 grudnia 2015
 */

#include <stdio.h>
#include <stdlib.h>

#define ROZMIAR_POCZATKOWY 10

#define STR_PRZEPELNIENIE_PAMIECI "Blad: przepelnienie pamieci"

/**
 * Pisze na wyjście diagnostyczne komunikat o przepełnieniu pamięci
 * i przerywa wykonanie programu.
 */
void blad_przepelnienie_pamieci(void)
{
    fprintf(stderr, "%s\n", STR_PRZEPELNIENIE_PAMIECI);
    exit(EXIT_FAILURE);
}

/**
 * Pisze permutację kolumn, przenoszącą na początek te kolumny, w których są
 * mniej niż dwa znaki różne od spacji.
 *
 * W tablicy @a rozmiaru @n są liczby znaków różnych od spacji w odpowiednich
 * kolumnach tekstu wejściowego.
 */
void pisz(int a[], int n)
{
    int w;

    for (w = 0; w < n; ++w) {
        if (a[w] < 2) {
            printf(" %d", w);
        }
    }
    for (w = 0; w < n; ++w) {
        if (a[w] >= 2) {
            printf(" %d", w);
        }
    }
    putchar('\n');
}

/**
 * Powiększa dwukrotnie tablicę @a rozmiaru @rozmiar. Aktualizuje oba
 * argumenty. Zgłasza błąd, gdy zabraknie pamięci.
 */
void powieksz(int **a, int *rozmiar)
{
    int *b;
    int nowy = 2 * *rozmiar;

    if ((b = realloc(*a, nowy * sizeof *b)) == NULL) {
        free(*a);
        blad_przepelnienie_pamieci();
    }
    *a = b;
    while (*rozmiar < nowy) {
        (*a)[(*rozmiar)++] = 0;
    }
}

/**
 * Dla tekstu z wejścia generuje permutację kolumn.
 */
int main(void)
{
    int *a;
    int rozmiar = ROZMIAR_POCZATKOWY;
    int c, w, max, i;

    if ((a = malloc(rozmiar * sizeof *a)) == NULL) {
        blad_przepelnienie_pamieci();
    }
    for (i = 0; i < rozmiar; ++i) {
        a[i] = 0;
    }
    max = w = 0;
    while ((c = getchar()) != EOF) {
        if (c == '\n') {
            w = 0;
        } else {
            ++w;
            if (w > max) {
                max = w;
            }
            if (w > rozmiar) {
                powieksz(&a, &rozmiar);
            }
            if (c != ' ') {
                ++a[w - 1];
            }
        }
    }
    pisz(a, max);
    free(a);
    return EXIT_SUCCESS;
}
