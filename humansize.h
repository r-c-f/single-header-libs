/* humansize
 * humn sizing/scaling functions
 *
 * Version 2.1
 *
 * Copyright 2023 Ryan Farley <ryan.farley@gmx.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR
 * N CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#if !defined(HUMANSIZEH_INCLUDE)
#define HUMANSIZEH_INCLUDE

#if !defined(SHL_UNUSED) /* because someone might have their own */
	#if (defined(__cplusplus) && (__cplusplus >= 201703L)) ||\
    	(defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 202311L))
		#define SHL_UNUSED [[maybe_unused]]
	#elif defined(__GNUC__)
		#define SHL_UNUSED __attribute__((unused))
	#else
		#define SHL_UNUSED
	#endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* this is necessary when scaling upward; it may be defined externally to
 * suit your tastes in precision or lack thereof */
#if !defined(HUMANSIZE_EPSILON)
#define HUMANSIZE_EPSILON 1e-8L
#endif

/* on the chance that you despise all unicode, or want a different code point,
 * define your mu here */
#if !defined(HUMANSIZE_MU)
#define HUMANSIZE_MU "µ"
#endif


/* Decimal and binary prefix sets */
struct humansize_preset {
	/* factor -- positive for divisor, negative for multiplier */
	double factor;
	/* NULL-terminated array of prefix strings, starting with "" */
	char **pre;
};


SHL_UNUSED static struct humansize_preset humansize_cust = {
	.factor = 1024.L,
	.pre = (char *[]){
		"",
		"K",
		"M",
		"G",
		"T",
		"P",
		"E",
		"Z",
		"Y",
		NULL
	},
};
SHL_UNUSED static struct humansize_preset humansize_si = {
	.factor = 1000.L,
	.pre = (char *[]){
		"",
		"k",
		"M",
		"G",
		"T",
		"P",
		"E",
		"Z",
		"Y",
		NULL
	},
};
SHL_UNUSED static struct humansize_preset humansize_si_up = {
	.factor = -1000.L,
	.pre = (char *[]){
		"",
		"m",
		HUMANSIZE_MU,
		"n",
		"p",
		"f",
		"a",
		"z",
		"y",
		NULL,
	},
};
SHL_UNUSED static struct humansize_preset humansize_iec = {
	.factor = 1024.L,
	.pre = (char *[]){
		"",
		"Ki",
		"Mi",
		"Gi",
		"Ti",
		"Pi",
		"Ei",
		"Zi",
		"Yi",
		NULL
	},
};

/* to avoid -lm */
#define HUMANSIZE_ABS(n) (((n) < 0.L) ? ((n) * -1.L) : (n))

/* scales a value to fit with a prefix, from a set selected with preset.
 * Scaled value is placed in res, selected prefix is placed in res_pre
 *
 * Returns:
 * 	0 if the result could not be fully reduced,
 * 	1 if the result could be fully reduced.
*/
SHL_UNUSED static int humansize_scale(long double n, struct humansize_preset *preset, long double *res, char **res_pre)
{
	int ret = 1;
	char **pre = preset->pre;
	long double factor = preset->factor;

	if (factor < 0) {
		factor *= -1;
		for (; *pre; ++pre) {
			if ((n > 1.L) || (HUMANSIZE_ABS((n * factor) - factor) < HUMANSIZE_EPSILON)) {
				break;
			}
			n *= factor;
		}
	} else {
		for (; *pre; ++pre) {
			if ((n / factor) < 1.) {
				break;
			}
			n /= factor;
		}
	}

	*res = n;

	if (!*pre) {
		--pre;
		ret = 0;
	}

	*res_pre = *pre;
	return ret;
}

/* scale a value up or down, depending on its value. Only SI makes sense here;
 * same as above, otherwise */
SHL_UNUSED static int humansize_scale_full(long double n, long double *res, char **res_pre)
{
	struct humansize_preset *preset = (n < 1.L) ? &humansize_si_up : &humansize_si;
	return humansize_scale(n, preset, res, res_pre);
}

/* Like sscanf. If base is 0, it will be assumed based on the full prefix. If
 * given as 2, even decimal prefixes will be assumed to be binary.
 *
 * Returns:
 * -	-1 if there is no number at all or base is invalid
 * - 	0 if there is no prefix detected
 * - 	1 if there is a prefix detected and the result is adjusted accordingly
*/
SHL_UNUSED static int humansize_parse(const char *s, int base, long double *res)
{
	char *pre = "kmgtpezy";
	char *end = NULL;
	int i;
	long double mult;

	*res = strtold(s, &end);
	if (end == s) {
		return -1;
	}

	/* trim off any whitespace */
	for (; *end && isblank(*end); ++end);

	for (i = 0; pre[i]; ++i) {
		if (tolower(*end) == pre[i]) {
			if (!base) {
				if (*(end + 1) == 'i') {
					base = 2;
				} else {
					base = 10;
				}
			}
			break;
		}
	}
	if (!pre[i]) {
		return 0;
	}

	if (base == 2) {
		mult = 1024;
	} else if (base == 10) {
		mult = 1000;
	} else {
		return -1;
	}

	for (; i > -1; --i) {
		*res *= mult;
	}

	return 1;
}

#if defined(SHL_UNUSED)
	#undef SHL_UNUSED
#endif /* defined(SHL_UNUSED) */

#endif /* !defined(HUMANSIZEH_INCLUDE) */

