/* unused 
 * Pick the right attribute to silence unused function/variable errors
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
 * N CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#if !defined(UNUSEDH_INCLUDE)
#define UNUSEDH_INCLUDE

#if !defined(UNUSED) /* because someone might have their own */
	#if (defined(__cplusplus) && (__cplusplus >= 201703L)) ||\
    	(defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 202311L))
		#define UNUSED [[maybe_unused]]
	#elif defined(__GNUC__)
		#define UNUSED __attribute__((unused))
	#else
		#define UNUSED
	#endif
#endif

#endif /* !defined(UNUSEDH_INCLUDE) */
