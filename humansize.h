/* human size function
 *
 * Version 1.3
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


/* Decimal and binary prefixes */
static char *humansize_dec_pre[] = {
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
static char *humansize_bin_pre[] = {
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

/* scales a value to a fit with a prefix. base selects:
 * 	-2: 	binary, using single-letter SI prefixes
 * 	2: 	binary, usinng two-letter IEC prefixes
 * 	10: 	decimal, using single-letter SI prefixes
 *
 * Returns:
 * 	-1 if the base is invalid (result will not be reduced),
 * 	0 if the result could not be fully reduced,
 * 	1 if the result could be fully reduced.
*/
static int humansize_scale(double n, int base, double *res, char **res_pre)
{
	double div;
	char **pre;
	int ret = 1;

	switch (base) {
	case 2:
		pre = humansize_bin_pre;
		div = 1024.;
		break;
	case -2:
		pre = humansize_dec_pre;
		div = 1024.;
		break;
	case 10:
		pre = humansize_dec_pre;
		div = 1000.;
		break;
	default:
		*res_pre = "";
		*res = n;
		return -1;
	}

        for (; *pre; ++pre) {
                if ((n / div) < 1.)
                        break;
                n /= div;
        }

	*res = n;

	if (!*pre) {
		--pre;
		ret = 0;
	}

	*res_pre = *pre;
	return ret;
}

/* Like sscanf. If base is 0, it will be assumed based on the full prefix. If
 * given as 2, even decimal prefixes will be assumed to be binary.
 *
 * Returns:
 * -	-1 if there is no number at all or base is invalid
 * - 	0 if there is no prefix detected
 * - 	1 if there is a prefix detected and the result is adjusted accordingly
*/
static int humansize_parse(const char *s, int base, double *res)
{
	char *pre = "kmgtpezy";
	char *end = NULL;
	int i;
	double mult;

	*res = strtod(s, &end);
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
