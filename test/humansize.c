#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "../humansize.h"


void print_usage(char *name)
{
	fprintf(stderr,
		"Usage: \n" \
		"%s scale cust|iec|si val\n" \
		"%s scale_full val\n" \
		"%s parse base val\n",
		name, name, name);
}

char **preset_from_str(char *str)
{
	if (!strcasecmp(str, "cust"))
		return humansize_cust;
	if (!strcasecmp(str, "iec"))
		return humansize_iec;
	if (!strcasecmp(str, "si"))
		return humansize_si;
	if (!strcasecmp(str, "si_up"))
		return humansize_si_up;
	fprintf(stderr, "Uknown prefix type %s\n", str);
	exit(EXIT_FAILURE);
	/* unreachable */
	return NULL;
}

int main(int argc, char **argv)
{
	int base, ret;
	long double n, res;
	char *pre, *s;
	char **preset;

	if (argc < 3) {
		print_usage(argv[0]);
		return EXIT_FAILURE;
	}

	if (!strcmp(argv[1], "scale")) {
		if (argc != 4) {
			print_usage(argv[0]);
			return EXIT_FAILURE;
		}
		n = strtod(argv[3], NULL);
		preset = preset_from_str(argv[2]);
		if (!preset) {
			fprintf(stderr, "Uknown prefix set %s\n", argv[2]);
			print_usage(argv[0]);
			return EXIT_FAILURE;
		}
		ret = humansize_scalel(n, preset, &res, &pre);
	} else if (!strcmp(argv[1], "scale_full")) {
		n = strtod(argv[2], NULL);
		ret = humansize_scalel_full(n, &res, &pre);
	} else if (!strcmp(argv[1], "parse")) {
		if (argc == 3) {
			base = 0;
			s = argv[2];
		} else {
			base = strtol(argv[2], NULL, 0);
			s = argv[3];
		}
		ret = humansize_parse(s, base, &res);
		pre = "";
	} else {
		fprintf(stderr, "Uknown operation %s\n", argv[1]);
		print_usage(argv[0]);
		return EXIT_FAILURE;
	}

	printf("%Lf%s\n", res, pre);

	return ret ? EXIT_FAILURE : EXIT_SUCCESS;
}

