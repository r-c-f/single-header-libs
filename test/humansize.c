#include <stdio.h>
#include <stdlib.h>
#include "../humansize.h"


int main(int argc, char **argv)
{
	int base, ret;
	long double in, res;
	char *pre;

	if (argc != 3) {
		fprintf(stderr, "Usage: humansize [base] [value]\n");
		return EXIT_FAILURE;
	}

	base = strtol(argv[1], NULL, 0);
	in = strtod(argv[2], NULL);
	ret = humansize_scale(in, base, &res, &pre);
	fprintf(stderr, "returned %d\n", ret);
	printf("%Lf %s\n", res, pre);
	return ret;
}

