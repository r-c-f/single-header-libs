#define _POSIX_C_SOURCE 200809L
#include "../ssb.h"
#include "../humansize.h"
#include <assert.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>

static void print_ssb_info(struct ssb *s)
{
	long double size, pos;
	char *size_pre, *pos_pre;

	humansize_scale(s->pos, &humansize_iec, &pos, &pos_pre);
	humansize_scale(s->size, &humansize_iec, &size, &size_pre);

	fprintf(stderr, "\t\tposition %Lf%sB, size %Lf%sB\n",
			pos, pos_pre, size, size_pre);
}

static void print_memstream_info(FILE *f, size_t buf_size)
{
	long double size, pos;
	char *size_pre, *pos_pre;

	fflush(f);
	humansize_scale(ftell(f), &humansize_iec, &pos, &pos_pre);
	fclose(f);
	humansize_scale(buf_size, &humansize_iec, &size, &size_pre);
	fprintf(stderr, "\t\tposition %Lf%sB, size %Lf%sB\n",
			pos, pos_pre, size, size_pre);
}

static bool memstream_printf_test(int max)
{
	char *buf = NULL;
	size_t size = 0;
	FILE *f;
	int i;
	bool ret = true;

	if (!(f = open_memstream(&buf, &size))) {
		perror("open_memstream");
		return false;
	}

	for (i = 0; i < max; ++i) {
		if (fprintf(f, "This is a test write, iteration %d\n", i) == -1) {
			ret = false;
			break;
		}
	}
	print_memstream_info(f, size);
	free(buf);
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

static bool memstream_putc_test(int max)
{
	char *buf = NULL;
	size_t size = 0;
	FILE *f;
	int i, j;
	bool ret = true;
	char test[] = {'t', 'e', 's', 't', ' '};

	if (!(f = open_memstream(&buf, &size))) {
		perror("open_memstream");
		return false;
	}

	for (i = 0; i < max; ++i) {
		for (j = 0; j < sizeof(test); ++j) {
			if (putc(test[j], f) == EOF) {
				ret = false;
				break;
			}
		}
	}
	print_memstream_info(f, size);
	free(buf);
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

static bool memstream_getline_test(int max, FILE *in)
{
	char *line = NULL;
	char *buf = NULL;
	size_t buf_size = 0;
	size_t line_size = 0;
	ssize_t read_count;
	int i;
	FILE *f;
	bool ret = true;

	if (!(f = open_memstream(&buf, &buf_size))) {
		perror("open_memstream");
		return false;
	}

	for (i = 0; i < max; ++i) {
		if ((read_count = getline(&line, &line_size, in)) == -1) {
			perror("getline");
			ret = false;
			break;
		}
		rewind(in);
		if (!fwrite(line, 1, read_count, f)) {
			perror("fwrite");
			ret = false;
			break;
		}
	}
	print_memstream_info(f, buf_size);
	free(buf);
	free(line);
	return ret;
}

static void print_usage(char *name)
{
	fprintf(stderr, "Usage: %s [ops]\n", name);
}

static void print_time_diff(struct timeval *start, struct timeval *end)
{
	long double d;
	long double d_scale;
	char *d_pre;

	d = (long double)end->tv_sec + ((long double)end->tv_usec * 1e-6);
	d -= (long double)start->tv_sec + ((long double)start->tv_usec * 1e-6);

	humansize_scale_full(d, &d_scale, &d_pre);

	fprintf(stderr, "\t\t\tTIME: %Lf%ss\n", d_scale, d_pre);
}



#define ARRAYLEN(a) (sizeof(a) / sizeof(*a))

#define RUN_TEST(func,...) \
	do { \
		struct timeval start, end; \
		fprintf(stderr, "\t%s... \n", #func); \
		gettimeofday(&start, NULL); \
		if (!func(__VA_ARGS__)) { \
			fprintf(stderr, "\tFAILED\n");\
			return EXIT_FAILURE;\
		}\
		gettimeofday(&end, NULL); \
		print_time_diff(&start, &end); \
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
		RUN_TEST(memstream_printf_test, iter_count);
		RUN_TEST(ssb_addc_test, iter_count, grow[i]);
		RUN_TEST(memstream_putc_test, iter_count);
		RUN_TEST(ssb_getline_test, iter_count, grow[i], f);
		RUN_TEST(memstream_getline_test, iter_count, f);
	}

	return EXIT_SUCCESS;
}

