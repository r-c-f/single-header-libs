/* humansize
 * humn sizing/scaling functions
 *
 * Version 4.0-8
 *
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
#define HUMANSIZE_EPSILON 1e-8
#endif

/* on the chance that you despise all unicode, or want a different code point,
 * define your mu here */
#if !defined(HUMANSIZE_MU)
#define HUMANSIZE_MU "µ"
#endif



/* prefix sets. The first string is a single character long, as follows:
 * - d for 1000,
 * - b for 1024,
 * - D for 1/1000
 *
 * followed by a set of prefixes for increasing/decreasing values, starting
 * with ""
*/
#define HUMANSIZE_MUL_1000 "d"
#define HUMANSIZE_MUL_1024 "b"
#define HUMANSIZE_DIV_1000 "D"
SHL_UNUSED static char *humansize_cust[] = {
	HUMANSIZE_MUL_1024,
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
};

SHL_UNUSED static char *humansize_si[] = {
	HUMANSIZE_MUL_1000,
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
};

SHL_UNUSED static char *humansize_si_up[] = {
	HUMANSIZE_DIV_1000,
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
};

SHL_UNUSED static char *humansize_iec[] = {
	HUMANSIZE_MUL_1024,
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
};

/* to avoid -lm */
#define HUMANSIZE_ABS(n) (((n) < 0.F) ? ((n) * -1.F) : (n))


/* because I'm now using this on systems with unspeakably-horrible foating
 * point support that would violate the standard but exists anyway...
 *
 * the functions are created with macros. The suffix/infix will be:
 *  f for float,
 *  d for double,
 *  l for long double
 *
 * If available, generic versions without any type notation will exist, based
 * either on __STDC_VERSION__ indicating C11, or the HAVE_GENERIC macro being
 * defined.
*/



/* scales a value to fit with a prefix, from a set selected with preset.
 * Scaled value is placed in res, selected prefix is placed in res_pre
 *
 * Returns:
 * 	0 if the result could not be fully reduced,
 * 	1 if the result could be fully reduced.
 *
*/
#define HUMANSIZE_SCALE_T(T, name) SHL_UNUSED static int name(T n, char **pre, T *res, char **res_pre)\
{\
	int ret = 1;\
	int div = 0;\
	T factor; \
	switch (pre[0][0]) {\
		case 'd':\
			factor = 1000;\
			break;\
		case 'b':\
			factor = 1024;\
			break;\
		case 'D':\
			factor = 1000;\
			div = 1;\
			break;\
		default:\
			*res_pre = "";\
			*res = n;\
			return 0;\
	}\
	++pre;\
\
	if (div) {\
		for (; *pre; ++pre) {\
			if ((n > 1.) || (HUMANSIZE_ABS((n * factor) - factor) < HUMANSIZE_EPSILON)) {\
				break;\
			}\
			n *= factor;\
		}\
	} else {\
		for (; *pre; ++pre) {\
			if ((n / factor) < 1.) {\
				break;\
			}\
			n /= factor;\
		}\
	}\
\
	*res = n;\
\
	if (!*pre) {\
		--pre;\
		ret = 0;\
	}\
\
	*res_pre = *pre;\
	return ret;\
}
HUMANSIZE_SCALE_T(float, humansize_scalef)
HUMANSIZE_SCALE_T(double, humansize_scaled)
HUMANSIZE_SCALE_T(long double, humansize_scalel)
#undef HUMANSIZE_SCALE_T

/* scale a value up or down, depending on its value. Only SI makes sense here;
 * same as above, otherwise */
#define HUMANSIZE_SCALE_T_FULL(T, hsname) SHL_UNUSED static int hsname##_full(T n, T *res, char **res_pre)\
{\
	char **preset = (n < 1.F) ? humansize_si_up : humansize_si;\
	return hsname(n, preset, res, res_pre);\
}
HUMANSIZE_SCALE_T_FULL(float, humansize_scalef)
HUMANSIZE_SCALE_T_FULL(double, humansize_scaled)
HUMANSIZE_SCALE_T_FULL(long double, humansize_scalel)
#undef HUMANSIZE_SCALE_T_FULL

/* Like sscanf. If base is 0, it will be assumed based on the full prefix. If
 * given as 2, even decimal prefixes will be assumed to be binary.
 *
 * Returns:
 * -	-1 if there is no number at all or base is invalid
 * - 	0 if there is no prefix detected
 * - 	1 if there is a prefix detected and the result is adjusted accordingly
*/
#define HUMANSIZE_PARSE_T(T, name, conv_func) SHL_UNUSED static int name(const char *s, int base, T *res)\
{\
	char *pre = "kmgtpezy";\
	char *end = NULL;\
	int i;\
	T mult;\
\
	*res = conv_func(s, &end);\
	if (end == s) {\
		return -1;\
	}\
\
	/* trim off any whitespace */\
	for (; *end && isblank(*end); ++end);\
\
	for (i = 0; pre[i]; ++i) {\
		if (tolower(*end) == pre[i]) {\
			if (!base) {\
				if (*(end + 1) == 'i') {\
					base = 2;\
				} else {\
					base = 10;\
				}\
			}\
			break;\
		}\
	}\
	if (!pre[i]) {\
		return 0;\
	}\
\
	if (base == 2) {\
		mult = 1024;\
	} else if (base == 10) {\
		mult = 1000;\
	} else {\
		return -1;\
	}\
\
	for (; i > -1; --i) {\
		*res *= mult;\
	}\
\
	return 1;\
}
HUMANSIZE_PARSE_T(float, humansize_parsef, strtof)
HUMANSIZE_PARSE_T(double, humansize_parsed, strtod)
HUMANSIZE_PARSE_T(long double, humansize_parsel, strtold)
#undef HUMANSIZE_PARSE_T


/* and the generics, if available */
#if defined(HAVE_GENERIC) || (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201000L))
#define humansize_scale(n, preset, res, res_pre) _Generic((res), \
		float *: humansize_scalef,\
		double *: humansize_scaled, \
		long double *: humansize_scalel \
	)(n, preset, res, res_pre)
#define humansize_scale_full(n, res, res_pre) _Generic((res), \
		float *: humansize_scalef_full,\
		double *: humansize_scaled_full, \
		long double *: humansize_scalel_full \
	)(n, res, res_pre)
#define humansize_parse(s, base, res) _Generic((res), \
		float *: humansize_parsef,\
		double *: humansize_parsed, \
		long double *: humansize_parsel \
	)(s, base, res)
#endif

#if defined(SHL_UNUSED)
	#undef SHL_UNUSED
#endif /* defined(SHL_UNUSED) */

#endif /* !defined(HUMANSIZEH_INCLUDE) */

