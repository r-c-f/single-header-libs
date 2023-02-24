/* human size function
 *
 * Version 1.0
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

#endif
