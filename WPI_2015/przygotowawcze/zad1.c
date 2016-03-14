/*
 *
 * Zastąp każdy element tablicy przed dodanie jego wartości
 * do sumy wartości wszystkich elementów go poprzedzających
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
	for(i = 1; i < n; i++)
		tab[i] += tab[i-1];

	printf("%d ", n);
	for(i = 0; i < n; i++)
		printf("%d ", tab[i]);
	printf("\n");

}
