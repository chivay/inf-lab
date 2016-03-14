/**
 * "Permutacje"
 *
 * Program przykładowy do zajęć laboratoryjnych ze Wstępu do programowania.
 *
 * Wywołany z jednym argumentem S pisze treść pliku ".x", na podstawie
 * którego program x wygeneruje wszystkie permutacje napisu S.
 *
 * Ograniczenie: argument nie powinien zawierać spacji.
 *
 * Sposób uruchomienia:
 *
 * $ ./permutacje abcde >permutacje.x
 * $ ./x <permutacje.x | sort >permutacje.xx
 * $ cut -c-5 <permutacje.xx >permutacje.out
 *
 * lub krócej:
 *
 * ./permutacje abcde | ./x | sort | cut -c-5 >permutacje.out
 *
 * autor: Artur Zaroda <zaroda@mimuw.edu.pl>
 * wersja: 1.0
 * data: 14 grudnia 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WYKORZYSTANA 'x'

#define STR_BLAD "Blad"
#define STR_PROGRAM_OCZEKUJE_JEDNEGO_ARGUMENTU \
        "program oczekuje jednego argumentu - permutowanego napisu"
#define STR_NAPIS_NIE_POWINIEN_ZAWIERAC_SPACJI \
        "napis nie powinien zawierac spacji"

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
 * Na podstawie argumentu programu generuje plik ".x".
 */
int main(int argc, char *argv[])
{
    int n, i, j;
    char *s;

    if (argc != 2) {
        blad(STR_PROGRAM_OCZEKUJE_JEDNEGO_ARGUMENTU);
    }
    s = argv[1];
    n = strlen(s);
    for (i = 0; i < n; ++i) {
        if (s[i] == ' ') {
            blad(STR_NAPIS_NIE_POWINIEN_ZAWIERAC_SPACJI);
        }
        for (j = 0; j < n; ++j) {
            printf("%*c%*c\n", j + 1, s[i], n - j + i, WYKORZYSTANA);
        }
    }
    return EXIT_SUCCESS;
}
