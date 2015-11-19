/*
 *
 * Odtwórz z tablicy otrzymanej w poprzednim zadaniu
 * oryginalne wartości.
 *
 */

#include <stdio.h>

#define MAX 10000

int tab[MAX];

int main() {
	int n;
	scanf("%d", &n);

	int i;
	for(i = 0; i < n; i++)
		scanf("%d", tab+i);

	// algo
	for(i = n-1; i > 0; i--)
		tab[i] -= tab[i-1];

	printf("%d ", n);
	for(i = 0; i < n; i++)
		printf("%d ", tab[i]);
	printf("\n");

}
