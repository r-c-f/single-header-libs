/* human size function
 *
 * Version 1.1
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

/* Like snprintf. base selects either decimal or binary prefixes for 10 or 2,
 * respectively. Output formatted N [PREFIX]UNIT */
static int humansize_print(char *buf, size_t size, double n, int base, char *unit)
{
	double div;
	char **pre;
	char *dec_pre[] = {
                "",
                "k",
                "M",
                "G",
                "T",
                "P",
                "E",
                NULL
        };
        char *bin_pre[] = {
                "",
                "Ki",
                "Mi",
                "Gi",
                "Ti",
                "Pi",
                "Ei",
                NULL
        };

	if (base == 2) {
		pre = bin_pre;
		div = 1024.;
	} else if (base == 10) {
		pre = dec_pre;
		div = 1000.;
	} else {
		return -1;
	}

        for (; *pre; ++pre) {
                if ((n / div) < 1.)
                        break;
                n /= div;
        }

	if (*pre) {
		return snprintf(buf, size, "%f %s%s", n, *pre, unit);
	}

	return -1;
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
	char *pre = "kmgtpe";
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
