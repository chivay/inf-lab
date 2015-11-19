/*
 *
 * Zastąp każdy element tablicy 
 * przez średnią arytmetyczną z sąsiadujacych z nim wartości
 * Załóż że pierwszy element jest sąsiadem ostatniego a ostatni pierwszego.
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

	tab2[0]   = (tab[n-1] + tab[1])/2;
	tab2[n-1] = (tab[n-2] + tab[0])/2;

	for(i = 1; i < n-1; i++)
		tab2[i] = (tab[i-1] + tab[i+1])/2;

	printf("%d ", n);
	for(i = 0; i < n; i++)
		printf("%d ", tab2[i]);
	printf("\n");

}
