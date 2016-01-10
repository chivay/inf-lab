/**
 * "Od środka"
 *
 * Program przykładowy do zajęć laboratoryjnych ze Wstępu do programowania.
 *
 * Wywołany z argumentem N będącym dziesiętnym zapisem liczby wierszy
 * kwadratowej szachownicy, pisze permutację pliku ".x". Przeniesie ona
 * na początek te kolumny pliku, które reprezentują decyzję umieszczenia
 * hetmana w określonym wierszu szachownicy i uporządkuje je w kolejności
 * rosnącej odległości od wiersza środkowego. Np. program wywołany z argumentem
 * "8" da wynik " 99 98 100 97 101 96 102 95 0 1 ... 94".
 *
 * autor: Artur Zaroda <zaroda@mimuw.edu.pl>
 * wersja: 1.0
 * data: 14 grudnia 2015
 */

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define STR_BLAD "Blad"
#define STR_ARGUMENTEM_POWINNA_BYC_LICZBA \
        "argumentem powinna byc liczba"
#define STR_PRZEKROCZENIE_ZAKRESU \
        "przekroczenie zakresu"
#define STR_PROGRAM_OCZEKUJE_JEDNEGO_ARGUMENTU \
        "program oczekuje jednego argumentu"

/**
 * Pisze na wyjście diagnostyczne komunikat o błędzie @s i przerywa wykonanie
 * programu.
 */
void blad(const char *s)
{
    fprintf(stderr, "%s: %s\n", STR_BLAD, s);
    exit(EXIT_FAILURE);
}

/**
 * Daje wartość całkowitą między @min a @max zapisaną dziesiętnie na @s.
 * W przypadku wykrycia błędu, przerywa wykonanie programu, zgłaszając
 * odpowiedni komunikat.
 */
int wartosc_dziesietna(char *s, int min, int max)
{
    long w;
    char *z;

    errno = 0;
    w = strtol(s, &z, 10);
    if (*z != '\0') {
        blad(STR_ARGUMENTEM_POWINNA_BYC_LICZBA);
    }
    if (errno != 0 || w < min || w > max) {
        blad(STR_PRZEKROCZENIE_ZAKRESU);
    }
    return w;
}

/**
 * Pisze permutację pliku ".x" dla szachownicy o rozmiarze określonym
 * argumentem programu.
 */
int main(int argc, char *argv[])
{
    int i, znak, aktualny, rozmiar, przesuniecie;

    if (argc != 2) {
        blad(STR_PROGRAM_OCZEKUJE_JEDNEGO_ARGUMENTU);
    }
    rozmiar = wartosc_dziesietna(argv[1], 0, INT_MAX);
    przesuniecie = rozmiar * (rozmiar + 4) - 1;
    aktualny = rozmiar / 2;
    znak = -1;
    for (i = 1; i <= rozmiar; ++i) {
        printf(" %d", aktualny + przesuniecie);
        aktualny += znak * i;
        znak = -znak;
    }
    for (i = 0; i < przesuniecie; ++i) {
        printf(" %d", i);
    }
    putchar('\n');
    return EXIT_SUCCESS;
}
