#include "../sopt.h"
#include "../xmem.h"

struct sopt optspec[] = {
	SOPT_INIT_ARGL('S', "str", SOPT_ARGTYPE_STR, "str", "A string argument"),
	SOPT_INIT_ARGL('s', "short", SOPT_ARGTYPE_SHORT, "n", "A short argument"),
	SOPT_INIT_ARGL('i', "int", SOPT_ARGTYPE_INT, "n", "A int argument"),
	SOPT_INIT_ARGL('l', "long", SOPT_ARGTYPE_LONG, "n", "A long argument"),
	SOPT_INIT_ARGL('L', "longlong", SOPT_ARGTYPE_LONGLONG, "n", "A long long argument"),
	SOPT_INIT_ARGL('f', "float", SOPT_ARGTYPE_FLOAT, "n", "A float argument"),
	SOPT_INIT_ARGL('d', "dbl", SOPT_ARGTYPE_DBL, "n", "A double argument"),
	SOPT_INIT_ARGL('D', "longdbl", SOPT_ARGTYPE_LONGDBL, "n", "A long double argument"),
	SOPT_INIT_END
};

int main(int argc, char **argv)
{
	int opt, optind = 0;
	union sopt_arg soptarg = {0};

	char *float_fmt, *dbl_fmt, *ldbl_fmt;

	xasprintf(&float_fmt, "Got float argument: %%%dg\n", FLT_DIG);
	xasprintf(&dbl_fmt, "Got double argument: %%%dg\n", DBL_DIG);
	xasprintf(&ldbl_fmt, "Got long double argument: %%%dLg\n", LDBL_DIG);

	sopt_usage_set(optspec, argv[0], "A test of argument parsing funtionality");
	while ((opt = sopt_getopt_s(argc, argv, optspec, NULL, &optind, &soptarg)) != -1) {
		switch (opt) {
			case 'S':
				printf("Got string argument: %s\n", soptarg.str);
				break;
			case 's':
				printf("Got short argument: %hd\n", soptarg.s);
				break;
			case 'i':
				printf("Got int argument: %d\n", soptarg.i);
				break;
			case 'l':
				printf("got long argument: %ld\n", soptarg.l);
				break;
			case 'L':
				printf("Got long long argument: %lld\n", soptarg.ll);
				break;
			case 'f':
				printf(float_fmt, soptarg.f);
				break;
			case 'd':
				printf(dbl_fmt, soptarg.d);
				break;
			case 'D':
				printf(ldbl_fmt, soptarg.ld);
				break;
			default:
				printf("Got unknown argument\n");
				sopt_usage_s();
				return 1;
		}
	}

	for (; optind < argc; ++optind) {
		printf("Got positional argument: %s\n", argv[optind]);
	}

	return 0;
}

