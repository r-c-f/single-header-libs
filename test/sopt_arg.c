#include "../sopt.h"


struct sopt optspec[] = {
	SOPT_INIT_ARGL('S', "str", SOPT_ARGTYPE_STR, "str", "A string argument"),
	SOPT_INIT_ARGL('s', "short", SOPT_ARGTYPE_SHORT, "n", "A short argument"),
	SOPT_INIT_ARGL('i', "int", SOPT_ARGTYPE_INT, "n", "A int argument"),
	SOPT_INIT_ARGL('l', "long", SOPT_ARGTYPE_LONG, "n", "A long argument"),
	SOPT_INIT_ARGL('L', "long long", SOPT_ARGTYPE_LONGLONG, "n", "A long long argument"),
	SOPT_INIT_ARGL('f', "float", SOPT_ARGTYPE_FLOAT, "n", "A float argument"),
	SOPT_INIT_END
};



int main(int argc, char **argv)
{
	int opt;
	union sopt_arg soptarg;
	
	sopt_usage_set(optspec, argv[0], "A test of argument parsing funtionality");
	while ((opt = sopt_getopt_s(argc, argv, optspec, NULL, NULL, &soptarg)) != -1) {
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
				printf("Got float argument: %f\n", soptarg.f);
				break;
		}
	}
	return 0;
}

