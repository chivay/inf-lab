/**
 * "Sudoku"
 *
 * Program przykładowy do zajęć laboratoryjnych ze Wstępu do programowania.
 *
 * Wywołany z jednym argumentem S, będącym zapisem łamigłówki Sudoku, pisze
 * treść pliku ".x", na podstawie którego program x wygeneruje rozwiązanie S.
 *
 * Łamigłówka jest reprezentowana przez ciąg 81 cyfr dziesiętnych opisujących,
 * wiersz po wierszu, stan kolejnych pól. Zero oznacza pole puste a pozostałe
 * cyfry to wartości pól zajętych.
 *
 * Wszystkie wiersze generowanego pliku ".x" są tej samej długości.
 *
 * Sposób uruchomienia:
 *
 * $ ./sudoku 640000020000070009090800700020504000003000400000302070008003060500060000030000098 >sudoku.x
 * $ ./x <sudoku.x >sudoku.xx
 * $ cut -c-81 <sudoku.xx | fold -9 >sudoku.out
 *
 * lub krócej:
 *
 * $ ./sudoku 640000020000070009090800700020504000003000400000302070008003060500060000030000098 | ./x | cut -c-81 | fold -9 >sudoku.out
 *
 * Jeśli chcemy użyć optymalizatora, wykonujemy:
 *
 * $ ./sudoku 640000020000070009090800700020504000003000400000302070008003060500060000030000098 >sudoku.x
 * $ ./mniej_niz_dwa <sudoku.x >sudoku.per
 * $ ./permutuj $(<sudoku.per) <sudoku.x >sudoku.xp
 * $ ./x <sudoku.xp >sudoku.xxp
 * $ ./permutuj - $(<sudoku.per) <sudoku.xxp >sudoku.xx
 * $ cut -c-81 <sudoku.xx | fold -9 >sudoku.out
 *
 * autor: Artur Zaroda <zaroda@mimuw.edu.pl>
 * wersja: 1.0
 * data: 14 grudnia 2015
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROZMIAR 9

#define WYKORZYSTANA 'x'

#define STR_BLAD "Blad"
#define STR_PROGRAM_OCZEKUJE_JEDNEGO_ARGUMENTU \
        "program oczekuje jednego argumentu: lamiglowki"
#define STR_ARGUMENTEM_POWINIEN_BYC_NAPIS_DLUGOSCI_81 \
        "argumentem powinien byc napis dlugosci 81"
#define STR_W_DANYCH_POWINNY_BYC_TYLKO_CYFRY_DZIESIETNE \
        "w danych powinny byc tylko cyfry dziesietne"

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
 * Pisze @z_ilu znaków. Znakiem @ktory, licząc od 0, jest @czym, pozostałe
 * to spacje.
 */
void oznacz(int z_ilu, int ktory, char czym)
{
    printf("%*c%*s", ktory + 1, czym, z_ilu - ktory - 1, "");
}

/**
 * Pisze wiersz pliku ".x" reprezentujący decyzję umieszczenia cyfry
 * o wartości @d w wierszu @w kolumny @k.
 */
void ustaw(int w, int k, int d)
{
    oznacz(ROZMIAR * ROZMIAR, w * ROZMIAR + k, d + '1');
    printf("%*s", d * 3 * ROZMIAR, "");
    oznacz(ROZMIAR, w, WYKORZYSTANA);
    oznacz(ROZMIAR, k, WYKORZYSTANA);
    oznacz(ROZMIAR, 3 * (w / 3) + k / 3, WYKORZYSTANA);
    printf("%*s", (ROZMIAR - d - 1) * 3 * ROZMIAR, "");
    printf("\n");
}

/**
 * Generuje plik ".x" dla łamigłówki będącej argumentem programu.
 */
int main(int argc, char *argv[])
{
    char *s;
    int w, k, i;

    if (argc != 2) {
        blad(STR_PROGRAM_OCZEKUJE_JEDNEGO_ARGUMENTU);
    }
    s = argv[1];
    if (strlen(s) != ROZMIAR * ROZMIAR) {
        blad(STR_ARGUMENTEM_POWINIEN_BYC_NAPIS_DLUGOSCI_81);
    }
    i = 0;
    for (w = 0; w < ROZMIAR; ++w) {
        for (k = 0; k < ROZMIAR; ++k) {
            char d = s[i++];
            if (!('0' <= d && d <= '9')) {
                blad(STR_W_DANYCH_POWINNY_BYC_TYLKO_CYFRY_DZIESIETNE);
            }
            if (d != '0') {
                ustaw(w, k, d - '1');
            } else {
                for (d = 0; d < ROZMIAR; ++d) {
                    ustaw(w, k, d);
                }
            }
        }
    }
    return EXIT_SUCCESS;
}
