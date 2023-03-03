/* simple string builder
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

#ifndef SSB_H_INC
#define SSB_H_INC
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>

struct ssb {
	char *buf;
	size_t size;
	size_t pos;
};

static int ssb_vprintf(struct ssb *s, const char *fmt, va_list ap)
{
	va_list sizeap;
	int size;
	size_t realloc_size;
	char *realloc_buf;

	if (!s) {
		errno = EINVAL;
		return -1;
	}

	va_copy(sizeap, ap);
	if ((size = vsnprintf(s->buf + s->pos, s->size - s->pos, fmt, sizeap)) >= s->size - s->pos) {
		realloc_size = size + s->size + 1;
		if (!(realloc_buf = realloc(s->buf, realloc_size))) {
			return -1;
		}
		s->buf = realloc_buf;
		s->size = realloc_size;
		vsnprintf(s->buf + s->pos, s->size - s->pos, fmt, ap);
	}
	va_end(sizeap);
	s->pos += size;

	return 0;
}

static int ssb_printf(struct ssb *s, const char *fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = ssb_vprintf(s, fmt, ap);
	va_end(ap);
	return ret;
}

static void ssb_rewind(struct ssb *s)
{
	s->pos = 0;
	if (s->buf) {
		*(s->buf) = '\0';
	}
}

static void ssb_free(struct ssb *s)
{
	s->pos = 0;
	if (s->buf) {
		free(s->buf);
	}
	s->size = 0;
	s->buf = NULL;
}

/* x variants -- can never fail! */
static void ssb_xvprintf(struct ssb *s, const char *fmt, va_list ap)
{
	if (ssb_vprintf(s, fmt, ap))
		abort();
}
static void ssb_xprintf(struct ssb *s, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	ssb_xvprintf(s, fmt, ap);
	va_end(ap);
}

#endif

