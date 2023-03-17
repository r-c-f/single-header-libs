#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../humansize.h"


int main(int argc, char **argv)
{
	int base, ret;
	long double n, res;
	char *pre, *s;

	if (argc != 4) {
		fprintf(stderr, "Usage: humansize [scale|scale_up|scale_full] [base] [value]\n");
		return EXIT_FAILURE;
	}

	base = strtol(argv[2], NULL, 0);
	s = argv[3];

	if (!strcmp(argv[1], "scale")) {
		n = strtod(s, NULL);
		ret = humansize_scale(n, base, &res, &pre);
		printf("%Lf %s\n", res, pre);
	} else if (!strcmp(argv[1], "scale_up")) {
		n = strtod(s, NULL);
		ret = humansize_scale_up(n, &res, &pre);
		printf("%Lf %s\n", res, pre);
	} else if (!strcmp(argv[1], "scale_full")) {
		n = strtod(s, NULL);
		ret = humansize_scale_full(n, &res, &pre);
		printf("%Lf %s\n", res, pre);
	} else if (!strcmp(argv[1], "parse")) {
		ret = humansize_parse(s, base, &res);
		printf("%LF\n", res);
	}
	fprintf(stderr, "returned %d\n", ret);

	return 0;
}

