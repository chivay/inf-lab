/**
 * "Permutuj"
 *
 * Program przykładowy do zajęć laboratoryjnych ze Wstępu do programowania.
 *
 * Przepisuje tekst z wejścia na wyjście, permutując kolumny w sposób określony
 * argumentami.
 *
 * Argumentami programu są numery kolumn tekstu wejściowego, liczone od 0,
 * wymienione w kolejności, jaką powinny mieć na wyjściu. Dodatkowo, jeśli
 * pierwszym argumentem jest minus, stosowana jest permutacja odwrotna do
 * podanej.
 *
 * Jeśli długość wiersza przekracza długość permutacji, "nadmiarowe" znaki
 * kopiowane są na wyjście bez zmiany kolejności. Wiersze krótsze niż
 * permutacja, również kopiowane są bez zmian.
 *
 * Wiersze na wyjściu zawsze są zakończone '\n' nawet, jeśli na wejściu po
 * ostatnim wierszu '\n' nie bylo.
 *
 * autor: Artur Zaroda <zaroda@mimuw.edu.pl>
 * wersja: 1.0
 * data: 14 grudnia 2015
 */

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BRAK (-1234)

#define ARG_ODWRACANIE "-"

#define STR_BLAD "Blad"

#define NIEPRAWIDLOWY_ZAPIS_LICZBY (-1)
#define STR_NIEPRAWIDLOWY_ZAPIS_LICZBY "nieprawidlowy zapis liczby"

#define PRZEKROCZENIE_ZAKRESU (-2)
#define STR_PRZEKROCZENIE_ZAKRESU "przekroczenie zakresu"

#define NIE_PERMUTACJA (-3)
#define STR_NIE_PERMUTACJA "to nie jest permutacja"

#define BRAK_MIEJSCA (-4)
#define STR_BRAK_MIEJSCA "zabraklo miejsca w pamieci"

/**
 * Daje komunikat o błędzie dla błędu o kodzie @n.
 */
char *komunikat(int n)
{
    switch(n) {
    case NIEPRAWIDLOWY_ZAPIS_LICZBY:
        return STR_NIEPRAWIDLOWY_ZAPIS_LICZBY;
    case PRZEKROCZENIE_ZAKRESU:
        return STR_PRZEKROCZENIE_ZAKRESU;
    case NIE_PERMUTACJA:
        return STR_NIE_PERMUTACJA;
    case BRAK_MIEJSCA:
        return STR_BRAK_MIEJSCA;
    default:
        assert(false);
        break;
    }
}

/**
 * Pisze na wyjście diagnostyczne komunikat o błędzie @n i przerywa wykonanie
 * programu.
 */
void blad(int n)
{
    fprintf(stderr, "%s: %s\n", STR_BLAD, komunikat(n));
    exit(EXIT_FAILURE);
}

/**
 * W tablicy @q zapisuje permutację odwrotną do permutacji @p, mającej
 * długość @n.
 */
void odwrotna(int p[], int n, int q[])
{
    int i;

    for (i = 0; i < n; ++i) {
        q[p[i]] = i;
    }
}

/**
 * Daje wartość liczby całkowitej między 0 a @max, zapisanej dziesiętnie na @s.
 * W przypadku stwierdzenia błędu, przekazuje liczbę ujemną będącą jego kodem.
 */
int napis_na_nieujemna(char *s, int max)
{
    long w;
    char *r;

    errno = 0;
    w = strtol(s, &r, 10);
    if (*r != '\0') {
        return NIEPRAWIDLOWY_ZAPIS_LICZBY;
    } else if (errno != 0 || w < 0 || w > max) {
        return PRZEKROCZENIE_ZAKRESU;
    } else {
        return w;
    }
}

/**
 * Kopiuje kolejne wiersze z wejścia na wyjście, stosując do ich prefiksu
 * permutację @p długości @n. Prefiks wejścia podlegający permutacji czyta do
 * bufora @s.
 */
void permutuj(int p[], int n, char *s)
{
    int c, i, j;

    i = 0;
    do {
        c = getchar();
        if (c != EOF || i > 0) {
            if (c == '\n' || c == EOF) {
                if (i < n) {
                    for (j = 0; j < i; ++j) {
                        putchar(s[j]);
                    }
                }
                putchar('\n');
                i = 0;
            } else if (i < n) {
                s[i++] = c;
                if (i == n) {
                    for (j = 0; j < n; ++j) {
                        putchar(s[p[j]]);
                    }
                }
            } else {
                putchar(c);
            }
        }
    } while (c != EOF);
}

/**
 * Do tablicy @p długości @n wpisuje permutację, której elementy zapisane
 * są dziesiętnie w tablicy @s. Wynikiem jest EXIT_SUCCESS lub kod błędu.
 */
int wpisz_permutacje(char *s[], int p[], int n)
{
    int i, a;

    for (i = 0; i < n; ++i) {
        p[i] = BRAK;
    }
    for (i = 0; i < n; ++i) {
        if ((a = napis_na_nieujemna(s[i], n - 1)) < 0) {
            return a;
        } else if (p[i] != BRAK) {
            return NIE_PERMUTACJA;
        }
        p[i] = a;
    }
    assert(i == n);
    return EXIT_SUCCESS;
}

/**
 * Kopiuje wejście na wyjście, permutując kolumny w sposób określony
 * argumentami programu.
 */
int main(int argc, char *argv[])
{
    int k, n, a;
    int *p = NULL;
    int *q = NULL;
    bool odwracanie = false;
    char *s = NULL;
    int rozmiar;

    n = argc - 1;
    k = 1;
    if (argc > 1 && strcmp(argv[1], ARG_ODWRACANIE) == 0) {
        odwracanie = true;
        --n;
        ++k;
    }
    rozmiar = (n > 0) ? n : 1;
    if ((p = malloc(rozmiar * sizeof *p)) == NULL) {
        blad(BRAK_MIEJSCA);
    }
    if ((a = wpisz_permutacje(argv + k, p, n)) != 0) {
        free(p);
        blad(a);
    }
    if (odwracanie) {
        if ((q = malloc(rozmiar * sizeof *p)) == NULL) {
            free(p);
            blad(BRAK_MIEJSCA);
        }
        odwrotna(p, n, q);
        free(p);
        p = q;
    }
    if ((s = malloc(rozmiar * sizeof *s)) == NULL) {
        free(p);
        blad(BRAK_MIEJSCA);
    }
    permutuj(p, n, s);
    free(s);
    free(p);
    return EXIT_SUCCESS;
}
