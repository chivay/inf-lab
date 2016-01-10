/**
 * "Mino"
 *
 * Program przykładowy do zajęć laboratoryjnych ze Wstępu do programowania.
 *
 * Czyta opis planszy oraz ciąg opisów figur i pisze treść pliku ".x". Na jego
 * podstawie program x wygeneruje wszystkie układy figur na planszy, nazywane
 * dalej rozwiązaniami. W rozwiązaniu figury pokrywają wszystkie wolne pola
 * planszy, każde dokładnie raz. Każda figura musi być użyta dokładnie raz.
 * Ponadto, figury mogą być obracane i odwracane w stosunku do postaci, jaką
 * miały na wejściu programu.
 *
 * Plansza jest reprezentowana przez ciąg wierszy zakończony wierszem pustym.
 * Znak kropki reprezentuje pole puste a każdy inny, w tym spacja, pole zajęte,
 * na którym nie można położyć figury.
 *
 * Figura również zapisana jest jako ciąg wierszy zakończony wierszem pustym.
 * Znak inny niż spacja w opisie figury jest jej częścią.
 *
 * Ograniczenia: w opisie planszy i figur nie powinny występować znaki
 * podkreślenia i tyldy.
 *
 * Sposób uruchomienia:
 *
 * $ ./mino <pentomino.txt >pentomino.x
 * $ ./x <pentomino.x | sort >pentomino.xx
 * $ cut -c-61 <pentomino.xx | fold -10 | tr -d _ | tr '~' ' ' >pentomino.out
 *
 * lub krócej:
 *
 * $ ./mino <pentomino.txt | ./x | sort | cut -c-61 | fold -10 | tr -d _ | tr '~' ' ' >pentomino.out
 *
 * Jeśli chcemy użyć optymalizatora, wykonujemy:
 *
 * $ ./mino <pentomino.txt >pentomino.x
 * $ ./przekatne 6 10 >pentomino.per
 * $ ./permutuj $(<pentomino.per) <pentomino.x >pentomino.xp
 * $ ./x <pentomino.xp >pentomino.xxp
 * $ ./permutuj - $(<pentomino.per) <pentomino.xxp | sort >pentomino.xx
 * $ cut -c-61 <pentomino.xx | fold -10 | tr -d _ | tr '~' ' ' >pentomino.out
 */

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_WIERSZE 100
#define MAX_KOLUMNY 200

#define N_SYMETRII 8

#define ZASTEPNIK_SPACJI '~'
#define SEPARATOR_PLANSZY '_'
#define POLE '.'
#define PUSTE ' '
#define ZNACZNIK_UZYTEJ 'x'

#define STR_BLAD "Blad"
#define STR_PUSTA_PLANSZA "pusta plansza"
#define STR_ZA_DUZO_WIERSZY "za duzo wierszy"
#define STR_ZA_DUZO_KOLUMN "za duzo kolumn"
#define STR_NIELEGALNY_ZNAK "nielegalny znak"

/**
 * Pisze na wyjście diagnostyczne komunikat o błędzie i przerywa wykonanie
 * programu.
 */
void blad(const char *s)
{
    fprintf(stderr, "%s: %s\n", STR_BLAD, s);
    exit(EXIT_FAILURE);
}

/**
 * Czyta, zakończony pustym wierszem lub końcem danych, opis planszy lub figury
 * i wpisuje go do @a. Na @nw zapisuje liczbę wierszy opisu a na @nk liczbę
 * kolumn. Dodatkowo, na @gw i @dw zapisuje numery pierwszego i ostatniego
 * wiersza, w których jest znak różny od spacji a na @lk i @pk numer pierwszej
 * i ostatniej takiej kolumny.
 */
void czytaj(char a[][MAX_KOLUMNY], int *nw, int *nk,
        int *gw, int *lk, int *dw, int *pk)
{
    int w, k, c;

    for (w = 0; w < MAX_WIERSZE; ++w) {
        for (k = 0; k < MAX_KOLUMNY; ++k) {
            a[w][k] = PUSTE;
        }
    }
    *nk = 0;
    *gw = MAX_WIERSZE;
    *lk = MAX_KOLUMNY;
    *dw = *pk = -1;
    w = 0;
    c = PUSTE;
    while (c != EOF && (c = getchar()) != EOF && c != '\n') {
        if (w >= MAX_WIERSZE) {
            blad(STR_ZA_DUZO_WIERSZY);
        }
        k = 0;
        do {
            if (k >= MAX_KOLUMNY) {
                blad(STR_ZA_DUZO_KOLUMN);
            }
            if (c == ZASTEPNIK_SPACJI || c == SEPARATOR_PLANSZY) {
                blad(STR_NIELEGALNY_ZNAK);
            }
            if (c != PUSTE) {
                a[w][k] = c;
                if (w < *gw) {
                    *gw = w;
                }
                if (k < *lk) {
                    *lk = k;
                }
                if (w > *dw) {
                    *dw = w;
                }
                if (k > *pk) {
                    *pk = k;
                }
            }
            ++k;
        } while ((c = getchar()) != EOF && c != '\n');
        if (k > *nk) {
            *nk = k;
        }
        ++w;
    }
    *nw = w;
    assert((*nw == 0) == (*nk == 0));
}

/**
 * Zamienia wartości zmiennych @a oraz @b.
 */
void zamien(int *a, int *b)
{
    int c;

    c = *a;
    *a = *b;
    *b = c;
}

/**
 * Daje "iterator", za pomocą którego można przechodzić w pętli po znakach
 * odpowiednio przekształconej figury. Wartość @x jest z przedziału od 0 do 7.
 * Trzy jej bity określają obrót i odwrócenie figury. Parametry (@gw, @lk)
 * określają współrzędne lewego górnego rogu figury a (@dw, @pk) to współrzędne
 * jej prawego dolnego roku. Na @w i @k funkcja dostaje adresy zmiennych
 * sterujących iteracji. Na zmiennych @sw i @bw zapisuje początkową i końcową
 * wartość zmiennej sterującej pętli przechodzącej po wierszach a na @zw zmianę
 * jej wartości w kolejnych obrotach pętli. Analogicznie, na @sk, @bk, @zk
 * przekazuje parametry iteracji po kolumnach. Zmienne @rw i @rk posłużą
 * użytkownikowi iteratora do odczytania numerów wiersza i kolumny.
 */
void iterator(
        int x,
        int gw, int lk, int dw, int pk,
        int *w, int *k,
        int *sw, int *sk, int *bw, int *bk, int *zw, int *zk,
        int **rw, int **rk)
{
    if ((x & 1) == 0) {
        *sw = gw;
        *bw = dw + 1;
        *zw = 1;
    } else {
        *sw = dw;
        *bw = gw - 1;
        *zw = -1;
    }
    if ((x & 2) == 0) {
        *sk = lk;
        *bk = pk + 1;
        *zk = 1;
    } else {
        *sk = pk;
        *bk = lk - 1;
        *zk = -1;
    }
    if ((x & 4) == 0) {
        *rw = w;
        *rk = k;
    } else {
        zamien(sw, sk);
        zamien(bw, bk);
        zamien(zw, zk);
        *rw = k;
        *rk = w;
    }
}

/**
 * Daje liczbę wierszy lub kolumn od @s do @b, z wyłączeniem tej drugiej,
 * w kierunku @z.
 */
int odleglosc(int s, int b, int z)
{
    return (b - s) * z;
}

/**
 * Na plik @f pisze wiersz ramki dla figury lub planszy o szerokości @n.
 */
void pisz_wiersz_ramki(FILE *f, int n)
{
    int i;

    fputc('+', f);
    for (i = 0; i < n; ++i) {
        fputc('-', f);
    }
    fputc('+', f);
    fputc('\n', f);
}

/**
 * Pisze na plik @f figurę lub planszę zapisaną w @a między wierzchołkami
 * (@gw, @lk) oraz (@dw, @pk), przekształconą zgodnie z @x. Parametr
 * @ramka wskazuje, czy wynik otoczyć ramką.
 */
void pisz(FILE *f, char a[][MAX_KOLUMNY],
        int gw, int lk, int dw, int pk, int x, bool ramka)
{
    int *rw, *rk;
    int w, k, sw, sk, bw, bk, zw, zk;

    iterator(x, gw, lk, dw, pk,
            &w, &k, &sw, &sk, &bw, &bk, &zw, &zk, &rw, &rk);
    if (ramka) {
        pisz_wiersz_ramki(f, odleglosc(sk, bk, zk));
    }
    for (w = sw; w != bw; w += zw) {
        if (ramka) {
            fputc('|', f);
        }
        for (k = sk; k != bk; k += zk) {
            fputc(a[*rw][*rk], f);
        }
        if (ramka) {
            fputc('|', f);
        }
        fputc('\n', f);
    }
    if (ramka) {
        pisz_wiersz_ramki(f, odleglosc(sk, bk, zk));
    }
}

/**
 * Sprawdza, czy figura w @f, między (@gw, @lk) oraz (@dw, @pk) przekształcona
 * zgodnie z parametrem @x jest identyczna z figurą po przekształceniu @y.
 */
bool identyczne(char f[][MAX_KOLUMNY], int gw, int lk, int dw, int pk,
        int x, int y)
{
    int *rw1, *rk1;
    int w1, k1, sw1, sk1, bw1, bk1, zw1, zk1;
    int *rw2, *rk2;
    int w2, k2, sw2, sk2, bw2, bk2, zw2, zk2;

    iterator(x, gw, lk, dw, pk,
            &w1, &k1, &sw1, &sk1, &bw1, &bk1, &zw1, &zk1, &rw1, &rk1);
    iterator(y, gw, lk, dw, pk,
            &w2, &k2, &sw2, &sk2, &bw2, &bk2, &zw2, &zk2, &rw2, &rk2);
    if (odleglosc(sw1, bw1, zw1) != odleglosc(sw2, bw2, zw2)) {
        return false;
    }
    if (odleglosc(sk1, bk1, zk1) != odleglosc(sk2, bk2, zk2)) {
        return false;
    }
    for (w1 = sw1, w2 = sw2; w1 != bw1 && w2 != bw2; w1 += zw1, w2 += zw2) {
        for (k1 = sk1, k2 = sk2; k1 != bk1 && k2 != bk2;
                k1 += zk1, k2 += zk2) {
            if (f[*rw1][*rk1] != f[*rw2][*rk2]) {
                return false;
            }
        }
        assert(k1 == bk1 && k2 == bk2);
    }
    assert(w1 == bw1 && w2 == bw2);
    return true;
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
 * Pisze planszę z @p, mającą @nw_plansza wierszy i @nk_plansza kolumn.
 */
void pisz_ustalone(char p[][MAX_KOLUMNY], int nw_plansza, int nk_plansza)
{
    int w, k;

    for (w = 0; w < nw_plansza; ++w) {
        for (k = 0; k < nk_plansza; ++k) {
            char c;
            if (p[w][k] == POLE) {
                c = PUSTE;
            } else if (p[w][k] == PUSTE) {
                c = ZASTEPNIK_SPACJI;
            } else {
                c = p[w][k];
            }
            putchar(c);
        }
    }
    putchar(SEPARATOR_PLANSZY);
    putchar('\n');
}

/**
 * Przygotowuje do wypelniania figurami planszę w @p, mającą @nw_plansza
 * wierszy i @nk_plansza kolumn. Wymazuje z niej wszystkie znaki z wyjątkiem
 * reprezentacji pól.
 */
void przygotuj_plansze(char p[][MAX_KOLUMNY], int nw_plansza, int nk_plansza)
{
    int w, k;

    for (w = 0; w < nw_plansza; ++w) {
        for (k = 0; k < nk_plansza; ++k) {
            if (p[w][k] != POLE) {
                p[w][k] = PUSTE;
            }
        }
    }
}

/**
 * Pisze wiersze pliku ".x" reprezentujące decyzje umieszczenia figury,
 * która w @f jest między wierzchołkami (@gw, @lk) oraz (@dw, @pk),
 * na planszy @p rozmiaru (@nw_plansza, @nk_plansza), w której pola puste
 * są między (@gw_plansza, @lk_plansza) a (@dw_plansza, @pk_plansza).
 * Parametr @x określa przekształcenie figury a @ktora to jej numer
 * kolejny.
 */
void kladz(
        char p[][MAX_KOLUMNY], int nw_plansza, int nk_plansza,
        int gw_plansza, int lk_plansza, int dw_plansza, int pk_plansza,
        char f[][MAX_KOLUMNY], int gw, int lk, int dw, int pk,
        int x, int ktora)
{
    int *rw, *rk;
    int w, k, sw, sk, bw, bk, zw, zk;
    int wysokosc, szerokosc;
    int cw, ck, qw, qk;

    iterator(x, gw, lk, dw, pk,
            &w, &k, &sw, &sk, &bw, &bk, &zw, &zk, &rw, &rk);
    wysokosc = odleglosc(sw, bw, zw);
    szerokosc = odleglosc(sk, bk, zk);
    for (cw = gw_plansza; cw + wysokosc - 1 <= dw_plansza; ++cw) {
        for (ck = lk_plansza; ck + szerokosc - 1 <= pk_plansza; ++ck) {
            bool pasuje = true;
            int zmienione = 0;
            for (w = sw, qw = cw; w != bw && pasuje; w += zw, ++qw) {
                for (k = sk, qk = ck; k != bk && pasuje; k += zk, ++qk) {
                    if (f[*rw][*rk] != PUSTE) {
                        if (p[qw][qk] != POLE) {
                            assert(p[qw][qk] == PUSTE);
                            pasuje = false;
                        } else {
                            ++zmienione;
                            p[qw][qk] = f[*rw][*rk];
                        }
                    }
                }
            }
            if (pasuje) {
                for (w = 0; w < nw_plansza; ++w) {
                    for (k = 0; k < nk_plansza; ++k) {
                        char c;
                        if (p[w][k] == POLE) {
                            c = PUSTE;
                        } else {
                            c = p[w][k];
                        }
                        putchar(c);
                    }
                }
                printf("%*c\n", ktora + 2, ZNACZNIK_UZYTEJ);
            }
            for (w = sw, qw = cw; w != bw && zmienione > 0; w += zw, ++qw) {
                for (k = sk, qk = ck; k != bk && zmienione > 0; k += zk, ++qk)
                    if (f[*rw][*rk] != PUSTE) {
                        assert(p[qw][qk] == f[*rw][*rk]);
                        p[qw][qk] = POLE;
                        --zmienione;
                    }
            }
            assert(zmienione == 0);
        }
    }
}

/**
 * Pisze treść pliku ".x" na podstawie czytanego z wejścia opisu planszy
 * i figur.
 */
int main(void)
{
    char plansza[MAX_WIERSZE][MAX_KOLUMNY];
    int nw_plansza, nk_plansza;
    int gw_plansza, lk_plansza, dw_plansza, pk_plansza;
    char figura[MAX_WIERSZE][MAX_KOLUMNY];
    int nw_figura, nk_figura;
    int gw_figura, lk_figura, dw_figura, pk_figura;
    int i, ktora;

    czytaj(plansza, &nw_plansza, &nk_plansza,
            &gw_plansza, &lk_plansza, &dw_plansza, &pk_plansza);
    if (nw_plansza == 0) {
        blad(STR_PUSTA_PLANSZA);
    }
#if DEBUG
    pisz(stderr, plansza, 0, 0, nw_plansza - 1, nk_plansza - 1, 0, false);
#endif
    pisz_ustalone(plansza, nw_plansza, nk_plansza);
    przygotuj_plansze(plansza, nw_plansza, nk_plansza);
    ktora = 0;
    for (;;) {
        czytaj(figura, &nw_figura, &nk_figura,
                &gw_figura, &lk_figura, &dw_figura, &pk_figura);
        if (nw_figura == 0) {
            break;
        }
        for (i = 0; i < N_SYMETRII; ++i) {
            bool bylo = false;
            int j;

            for (j = 0; j < i && !bylo; ++j) {
                bylo = identyczne(figura,
                        gw_figura, lk_figura, dw_figura, pk_figura, i, j);
            }
            if (!bylo) {
#if DEBUG
                pisz(stderr, figura,
                        gw_figura, lk_figura, dw_figura, pk_figura, i, true);
#endif
                kladz(plansza, nw_plansza, nk_plansza,
                        gw_plansza, lk_plansza, dw_plansza, pk_plansza,
                        figura, gw_figura, lk_figura, dw_figura, pk_figura,
                        i, ktora);
            }
        }
        ++ktora;
    }
    return EXIT_SUCCESS;
}
