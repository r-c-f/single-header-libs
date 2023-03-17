/* human size function
 *
 * Version 1.5
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
 * IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
#ifndef HUMANSIZE_H_INC
#define HUMANSIZE_H_INC
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* this is necessary when scaling upward; it may be defined externally to
 * suit your tastes in precision or lack thereof */
#if !defined(HUMANSIZE_EPSILON)
#define HUMANSIZE_EPSILON 1e-8L
#endif

/* Decimal and binary prefixes */
static char *humansize_dec[] = {
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
static char *humansize_dec_up[] = {
	"",
	"m",
	"μ",
	"n",
	"p",
	"f",
	"a",
	"z",
	"y",
	NULL
};
static char *humansize_bin[] = {
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
#define HUMANSIZE_ABS(n) (((n) < 0.L) ? ((n) * -1.L) : (n))
static int humansize_scale_(long double n, long double factor, char **pre, long double *res, char **res_pre)
{
	int ret = 1;

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

/* scales a value down to a fit with a prefix. base selects:
 * 	-2: 	binary, using single-letter SI prefixes
 * 	2: 	binary, usinng two-letter IEC prefixes
 * 	10: 	decimal, using single-letter SI prefixes
 *
 * Returns:
 * 	-1 if the base is invalid (result will not be reduced),
 * 	0 if the result could not be fully reduced,
 * 	1 if the result could be fully reduced.
*/
static int humansize_scale(long double n, int base, long double *res, char **res_pre)
{
	long double div;
	char **pre;
	int ret = 1;

	switch (base) {
	case 2:
		pre = humansize_bin;
		div = 1024.;
		break;
	case -2:
		pre = humansize_dec;
		div = 1024.;
		break;
	case 10:
		pre = humansize_dec;
		div = 1000.;
		break;
	default:
		*res_pre = "";
		*res = n;
		return -1;
	}

	return humansize_scale_(n, div, pre, res, res_pre);
}
/* scale a value up, rather than down. Only SI makes sense here, so no base
 * support; every other parameter is the same as for humansize_scale */
static int humansize_scale_up(long double n, long double *res, char **res_pre)
{
	return humansize_scale_(n, -1000, humansize_dec_up, res, res_pre);
}
/* scale a value up or down, depending on its value. Only SI makes sense here;
 * same as above. */
static int humansize_scale_full(long double n, long double *res, char **res_pre)
{
	if (n < 1.) {
		return humansize_scale_(n, -1000, humansize_dec_up, res, res_pre);
	}
	return humansize_scale_(n, 1000, humansize_dec, res, res_pre);
}

/* Like sscanf. If base is 0, it will be assumed based on the full prefix. If
 * given as 2, even decimal prefixes will be assumed to be binary.
 *
 * Returns:
 * -	-1 if there is no number at all or base is invalid
 * - 	0 if there is no prefix detected
 * - 	1 if there is a prefix detected and the result is adjusted accordingly
*/
static int humansize_parse(const char *s, int base, long double *res)
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



#endif
