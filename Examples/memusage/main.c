#include <stdio.h>
#include <stdlib.h>

#define CYCLES 20

int main(int argc, char *argv[])
{
	int i, j;
	size_t size;
	int *p;

	size = sizeof(*p) * 100;
	printf("malloc: %zu\n", size);
	p = malloc(size);

	for (i = 0; i < CYCLES; i++) {
		if (i < CYCLES / 2)
			j = i;
		else
			j--;

		size = sizeof(*p) * (j * 50 + 110);
		printf("realloc: %zu\n", size);
		p = realloc(p, size);

		size = sizeof(*p) * ((j + 1) * 150 + 110);
		printf("realloc: %zu\n", size);
		p = realloc(p, size);
	}

	free(p);
	exit(EXIT_SUCCESS);
}
