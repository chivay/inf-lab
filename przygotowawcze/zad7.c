/*
 *
 * Znajdź najmniejszą i największą wartość znajdującą się w tablicy
 *
 */
#include <stdio.h>

int main(void)
{
	int n, tmp, min, max;
	scanf("%d", &n);

	scanf("%d", &tmp);
	n--;

	min = max = tmp;

	while (n--)
	{
		scanf("%d", &tmp);
		min = tmp < min ? tmp : min;
		max = tmp > max ? tmp : max;
	}

	printf("%d %d\n", min, max);

	return 0;
}
