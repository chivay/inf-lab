/**
 * "Hetmany"
 *
 * Program przykładowy do zajęć laboratoryjnych ze Wstępu do programowania.
 *
 * Wywołany z jednym argumentem N pisze treść pliku ".x", na podstawie
 * którego program x wygeneruje wszystkie ustawienia N hetmanów na szachownicy
 * N na N, w których żadne dwa hetmany nie atakują się.
 *
 * Sposób uruchomienia:
 *
 * $ ./hetmany 10 >hetmany.x
 * $ ./x <hetmany.x | sort >hetmany.xx
 * $ cut -c-101 <hetmany.xx | fold -10 | tr -d _ >hetmany.out
 *
 * lub krócej:
 *
 * $ ./hetmany 10 | ./x | sort | cut -c-101 | fold -10 | tr -d _ >hetmany.out
 *
 * Jeśli chcemy użyć optymalizatora, wykonujemy:
 *
 * $ ./hetmany 10 >hetmany.x
 * $ ./od_srodka 10 >hetmany.per
 * $ ./permutuj $(<hetmany.per) <hetmany.x >hetmany.xp
 * $ ./x <hetmany.xp >hetmany.xxp
 * $ ./permutuj - $(<hetmany.per) <hetmany.xxp | sort >hetmany.xx
 * $ cut -c-101 <hetmany.xx | fold -10 |tr -d _ >hetmany.out
 *
 * autor: Artur Zaroda <zaroda@mimuw.edu.pl>
 * wersja: 1.0
 * data: 14 grudnia 2015
 */

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ZNACZNIK 'x'
#define HETMAN 'H'
#define SEPARATOR_PLANSZY '_'
#define WOLNE '-'

#define STR_BLAD "Blad"
#define STR_PROGRAM_OCZEKUJE_JEDNEGO_ARGUMENTU \
        "program oczekuje jednego argumentu - liczby hetmanow"
#define STR_ARGUMENTEM_POWINNA_BYC_LICZBA \
        "argumentem powinna byc liczba"
#define STR_PRZEKROCZENIE_ZAKRESU \
        "przekroczenie zakresu"

/**
 * Pisze komunikat o błędzie @s na wyjście diagnostyczne i przerywa
 * wykonanie programu.
 */
void blad(const char *s)
{
    fprintf(stderr, "%s: %s\n", STR_BLAD, s);
    exit(EXIT_FAILURE);
}

/**
 * Pisze @z_ilu znaków. Znakiem @ktory, licząc od 0, jest @czym, pozostałe
 * to spacje.
 */
void oznacz(int z_ilu, int ktory, char czym)
{
    printf("%*c%*s", ktory + 1, czym, z_ilu - ktory - 1, "");
}

/**
 * Jeśli @s to dziesiętny zapis liczby całkowitej między @min a @max,
 * daje jej wartość, w przeciwnym przypadku sygnalizuje błąd.
 */
int wartosc_dziesietnej(char *s, int min, int max)
{
    char *z;
    long w;

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
 * Na podstawie argumentu programu generuje plik ".x".
 */
int main(int argc, char *argv[])
{
    int n, w, k, i;

    if (argc != 2) {
        blad(STR_PROGRAM_OCZEKUJE_JEDNEGO_ARGUMENTU);
    }
    n = wartosc_dziesietnej(argv[1], 0, INT_MAX);
    i = 0;
    for (w = 0; w < n; ++w) {
        for (k = 0; k < n; ++k) {
            oznacz(n * n, i++, HETMAN);
            putchar(' ');
            oznacz(2 * n - 1, w + k, ZNACZNIK);
            oznacz(2 * n - 1, w - k + n - 1, ZNACZNIK);
            oznacz(n, w, ZNACZNIK);
            oznacz(n, k, ZNACZNIK);
            putchar('\n');
        }
    }
    for (k = 0; k < n * (n + 4) - 1; ++k) {
        if (k != n * n) {
            oznacz(n * (n + 6) - 1, k, WOLNE);
            putchar('\n');
        }
    }
    oznacz(n * (n + 6) - 1, n * n, SEPARATOR_PLANSZY);
    putchar('\n');
    return EXIT_SUCCESS;
}
