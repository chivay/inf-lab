/**
 * "Przekątne"
 *
 * Program przykładowy do zajęć laboratoryjnych ze Wstępu do programowania.
 *
 * Wywołany z argumentami W oraz K, będącymi dziesiętnym zapisem liczby wierszy
 * i liczby kolumn prostokątnej planszy, pisze permutację numerów znaków
 * w reprezentacji tej planszy.
 *
 * Zakłada, że plansza jest reprezentowana przez jeden wiersz tekstu, z polami
 * uporządkowanymi wierszami a w ramach wiersza kolumnami. Generowana
 * permutacja przestawia pola tak, by były uporządkowane po przekątnych
 * wyznaczonych sumą numeru wiersza i kolumny a w ramach przekątnej, po
 * rosnącym numerze wiersza. Np. program wywołany z argumentami "2 3" da wynik
 * "0 1 3 2 4 5".
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
#define STR_PROGRAM_OCZEKUJE_DWOCH_ARGUMENTOW \
        "program oczekuje dwoch argumentow"

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
 * Daje wartość minimalną z @a oraz @b.
 */
int min(int a, int b)
{
    return (a < b) ? a : b;
}

/**
 * Daje wartość maksymalną z @a oraz @b.
 */
int max(int a, int b)
{
    return (a < b) ? b : a;
}

/**
 * Pisze permutację reprezentacji planszy o rozmiarze określonym argumentami
 * programu.
 */
int main(int argc, char *argv[])
{
    int s, w, n_wierszy, n_kolumn;

    if (argc != 3) {
        blad(STR_PROGRAM_OCZEKUJE_DWOCH_ARGUMENTOW);
    }
    n_wierszy = wartosc_dziesietna(argv[1], 0, INT_MAX);
    n_kolumn = wartosc_dziesietna(argv[2], 0, INT_MAX);
    printf("%d", 0);
    for (s = 1; s < n_wierszy + n_kolumn - 1; ++s) {
        for (w = max(0, s - n_kolumn + 1); w < min(n_wierszy, s + 1); ++w) {
            printf(" %d", w * n_kolumn + s - w);
        }
    }
    putchar('\n');
    return EXIT_SUCCESS;
}
