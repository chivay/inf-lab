/*
 *
 * Znajdź wartość drugiego co do wielkości elementu z a (może sie powtarzać)
 *
 */
#include <stdio.h>

#define MAX 10000

int tab[MAX];

int main(void)
{
	int n;
	scanf("%d", &n);

	int i;
	for (i = 0; i < n; i++)
		scanf("%d", tab+i);
	int cnt = 1;
	int max = tab[0];

	for (i = 1; i < n; i++)
	{
		if (tab[i] == max)
			cnt++;
		else if (tab[i] > max)
		{
			max = tab[i];
			cnt = 1;
		}
	}
	// jesli mamy kilka powtorzen to wybieramy tę samą wartość
	if(cnt > 1)
	{
		printf("%d\n", max);
		return 0;
	}
	// inaczej musimy szukać dalej elementu < max
	int found = 0;
	int cand;
	//szukamy pierwszego potencjalnego kandydata (przewijamy maksy na poczatku)
	for (i = 0; i < n && !found; i++)
	{
		if (tab[i] < max)
		{
			cand = tab[i];
			found = 1;
		}
	}
	//szukamy dalej
	for (; i < n; i++)
	{
		if(tab[i] < max && tab[i] > cand)
			cand = tab[i];
	}
	printf("%d\n", cand);

	return 0;
}
