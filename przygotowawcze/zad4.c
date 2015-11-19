/*
 *
 * Jesli to możliwe, odtwórz oryginalne wartości z tablicy
 * uzyskanej w poprzednim zadaniu
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
	for (i = 0; i < n; i++)
		scanf("%d", tab+i);

	if (n % 2 != 0) {
		printf("Nie da się zrekonstruować!\n");
		return 1;
	}

	// pierwszą i ostatnią można przepisać
	tab2[0]   = tab[0];
	tab2[n-1] = tab[n-1];

	// idziemy po parzystych indeksach i naprawiamy 
	for (i = 2; i < n; i += 2)
		tab2[i] = 2*tab[i-1] - tab2[i-2];

	//idziemy po nieparzystych i naprawiamy
	for (i = n-1-2; i > 0; i -= 2)
		tab2[i] = 2*tab[i+1] - tab2[i+2];

	printf("%d ", n);
	for (i = 0; i < n; i++)
		printf("%d ", tab2[i]);
	printf("\n");

	return 0;
}
