#include "../ssb.h"
#include "../humansize.h"
#include <assert.h>
#include <string.h>

static void print_ssb_info(struct ssb *s)
{
	double size, pos;
	char *size_pre, *pos_pre;

	humansize_scale(s->pos, &humansize_iec, &pos, &pos_pre);
	humansize_scale(s->size, &humansize_iec, &size, &size_pre);

	fprintf(stderr, "\t\tgrow strategy %d: position %f%sB, size %f%sB\n", s->grow,
			pos, pos_pre, size, size_pre);
}

static bool ssb_getline_test(int max, enum ssb_grow grow, FILE *f)
{
	bool ret = true;
	struct ssb s = {grow};
	int i;

	for (i = 0; i < max; ++i) {
		if (!ssb_getline(&s, f)) {
			perror("FILE READ FAILED");
			ret = false;
			break;
		}
		rewind(f);
	}
	print_ssb_info(&s);
	ssb_free(&s);
	return ret;
}

static bool ssb_readfile_test(int max, enum ssb_grow grow, FILE *f)
{
	bool ret = true;
	struct ssb s = {grow};
	int i;

	for (i = 0; i < max; ++i) {
		if (!ssb_readfile(&s, f)) {
			perror("FILE READ FAILED");
			ret = false;
			break;
		}
		rewind(f);
	}
	print_ssb_info(&s);
	ssb_free(&s);
	return ret;
}

static bool ssb_printf_test(int max, enum ssb_grow grow)
{
	bool ret = true;
	struct ssb s = {grow};
	int i;

	for (i = 0; i < max; ++i) {
		if (ssb_printf(&s, "This is a test write, iteration %d\n", i) == -1) {
			ret = false;
			break;
		}
	}
	print_ssb_info(&s);
	ssb_free(&s);
	return ret;
}

static bool ssb_addc_test(int max, enum ssb_grow grow)
{
	bool ret = true;
	struct ssb s = {grow};
	int i, j;
	char test[] = {'t', 'e', 's', 't', ' '};

	for (i = 0; i < max; ++i) {
		for (j = 0; j < sizeof(test); ++j) {
			if (!ssb_addc(&s, test[j])) {
				ret = false;
				break;
			}
		}
	}
	print_ssb_info(&s);
	ssb_free(&s);
	return ret;
}

static void print_usage(char *name)
{
	fprintf(stderr, "Usage: %s [ops] \n", name);
}

#define ARRAYLEN(a) (sizeof(a) / sizeof(*a))

#define RUN_TEST(func,...) \
	do { \
		fprintf(stderr, "\t%s... \n", #func); \
		if (!func(__VA_ARGS__)) { \
			fprintf(stderr, "\tFAILED\n");\
			return EXIT_FAILURE;\
		}\
	} while(0)

int main(int argc, char **argv)
{
	int iter_count, i;
	enum ssb_grow grow[] = {
		SSB_GROW_1_5,
		SSB_GROW_2_0,
		SSB_GROW_EXACT
	};
	char *grow_name[] = {
		"SSB_GROW_1_5",
		"SSB_GROW_2_0",
		"SSB_GROW_EXACT",
	};
	FILE *f;

	assert(ARRAYLEN(grow) == ARRAYLEN(grow_name));

	if (argc != 2) {
		print_usage(argv[0]);
		return EXIT_FAILURE;
	}

	if (!(iter_count = atoi(argv[1]))) {
		fprintf(stderr, "Iteration count must not be zero\n");
		return EXIT_FAILURE;
	}

	if (!(f = tmpfile())) {
		perror("tmpfile");
		return EXIT_FAILURE;
	}
	fprintf(f, "test ");
	rewind(f);

	for (i = 0; i < ARRAYLEN(grow); ++i) {
		fprintf(stderr, "Trying strategy %s...\n", grow_name[i]);
		RUN_TEST(ssb_printf_test, iter_count, grow[i]);
		RUN_TEST(ssb_addc_test, iter_count, grow[i]);
		RUN_TEST(ssb_readfile_test, iter_count, grow[i], f);
		RUN_TEST(ssb_getline_test, iter_count, grow[i], f);
	}

	return EXIT_SUCCESS;
}

