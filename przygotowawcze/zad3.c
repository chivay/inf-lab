/*
 *
 * Zastąp każdy element tablicy poza pierwszym i ostatnim
 * przez średnią arytmetyczną z sąsiadujacych z nim wartości
 * Załóż że wszystkie wartości są parzyste
 *
 */

#include <stdio.h>

#define MAX 10000

int tab[MAX];
int tab2[MAX];
int main() {
	int n;
	scanf("%d", &n);

	int i;
	for(i = 0; i < n; i++)
		scanf("%d", tab+i);

	tab2[0]   = tab[0];
	tab2[n-1] = tab[n-1];

	for(i = 1; i < n-1; i++)
		tab2[i] = (tab[i-1] + tab[i+1])/2;

	printf("%d ", n);
	for(i = 0; i < n; i++)
		printf("%d ", tab2[i]);
	printf("\n");

}
