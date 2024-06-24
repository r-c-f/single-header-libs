/* xmem
 * memory/string functions that only fail catastrophically
 *
 * Version 1.5-2
 *
 *
 * Copyright 2021 Ryan Farley <ryan.farley@gmx.com>
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

#if !defined(XMEMH_INCLUDE)
#define XMEMH_INCLUDE

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
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

SHL_UNUSED
static void *xmalloc(size_t len)
{
	void *ret;
	if (!(ret = malloc(len)))
		abort();
	return ret;
}

SHL_UNUSED
static void *xcalloc(size_t nmemb, size_t size)
{
	void *ret;
	if (!(ret = calloc(nmemb, size)))
		abort();
	return ret;
}

SHL_UNUSED
static void *xrealloc(void *ptr, size_t len)
{
	if (!(ptr = realloc(ptr, len)))
		abort();
	return ptr;
}

/* xreallocarray: safely reallocate an array
 *
 * Like reallocarray() present on some platforms, will catch any possible
 * overflows */
SHL_UNUSED
static void *xreallocarray(void *ptr, size_t nmemb, size_t size)
{
	if (size && nmemb > SIZE_MAX / size) {
		abort();
	}
	return xrealloc(ptr, nmemb * size);
}

SHL_UNUSED
static char *xstrdup(const char *str)
{
	char *ret;
	size_t len;

	if (!str)
		return NULL;

	len = strlen(str) + 1;
	ret = xmalloc(len);
	return memcpy(ret, str, len);
}

/* x*asnprintf: print to a buffer, allocating/growing as needed
 *
 * If *size is 0, *strp is allocated anew; otherwise, it is grown to the
 * needed size. The current length of the string in the buffer is returned,
 * to allow for manual trimming if desired.
*/
SHL_UNUSED
static size_t xvasnprintf(char **strp, size_t *size, const char *fmt, va_list ap)
{
	va_list testap;
	int tsize;

	if (!*size) {
		*strp = NULL;
	}

	va_copy(testap, ap);
	if ((tsize = vsnprintf(*strp, *size, fmt, testap)) == -1)
		abort();
	va_end(testap);

	if ((size_t)tsize >= *size) {
		*size = (size_t)tsize + 1;
		*strp = xrealloc(*strp, *size);
		vsnprintf(*strp, *size, fmt, ap);
	}
	return tsize;
}

SHL_UNUSED
static size_t xasnprintf(char **strp, size_t *size, const char *fmt, ...)
{
	va_list ap;
	size_t ret;

	va_start(ap, fmt);
	ret = xvasnprintf(strp, size, fmt, ap);
	va_end(ap);

	return ret;
}

/* x*asprintf: print to a newly-allocated buffer
 *
 * A bit like the asprintf family on some platforms, but will work anywhere */
SHL_UNUSED
static void xvasprintf(char **strp, const char *fmt, va_list ap)
{
	size_t s = 0;
	xvasnprintf(strp, &s, fmt, ap);
}


SHL_UNUSED
static void xasprintf(char **strp, const char *fmt, ...)
{
	va_list ap;
	size_t s = 0;

	va_start(ap, fmt);
	xvasnprintf(strp, &s, fmt, ap);
	va_end(ap);
}

/* strfreev: free a NULL-terminated array of strings
 *
 * NULL-terminated array being a bit like argv in main(). */
SHL_UNUSED
static void strfreev(char **strv)
{
	if (strv) {
		size_t i;
		for (i = 0; strv[i]; ++i) {
			free(strv[i]);
		}
		free(strv);
	}
}

#if defined(SHL_UNUSED)
	#undef SHL_UNUSED
#endif /* defined(SHL_UNUSED) */

#endif /* !defined(XMEMH_INCLUDE) */

