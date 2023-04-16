divert(-1)

dnl My information:
define(`MY_NAME', `Ryan Farley')
define(`MY_MAIL', `ryan.farley@gmx.com')

dnl We use dnl for comments. # is dumb.
changecom()

dnl a way to include files without processing. Ugly? Yes. But it works, mostly
define(`uinclude', `syscmd(cat $1)')

dnl Translate to uppercase. Needed to generate the guard name
define(`LOWER', `abcdefghijklmnopqrstuvwxyz')
define(`UPPER', `ABCDEFGHIJKLMNOPQRSTUVWXYZ')
define(`toupper', `translit($1, LOWER, UPPER)')

dnl the actual start of the library: the name
define(`name', `define(`NAME', $1)'/* $1)
dnl and the description, if there is one
define(`desc', ` * $1')
dnl and the version
define(`vers', ` *
 * Version $1')
dnl and an MIT license: year
define(`mit', ` *
 * Copyright $1 MY_NAME <MY_MAIL>
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
*/'
)

dnl Include guard stuff
define(`GUARD_NAME', `toupper(NAME)H_INCLUDE')
define(`guard_start',`#if !defined(GUARD_NAME)
#`define' GUARD_NAME'
)
dnl And end the guard
define(`guard_end', `#endif /* !defined(GUARD_NAME) */')

dnl Include the base library
define(`base', `uinclude(NAME.h.in)')

dnl a bit of code to provide us with UNUSED....
define(`unused_start', `syscmd(`sed s/UNUSED/SHL_UNUSED/g <unused.h.in')')
dnl and to remove it if we would like
define(`unused_end', `#if defined(SHL_UNUSED)
	#undef SHL_UNUSED
#endif /* defined(SHL_UNUSED) */'
)

dnl Macros to handle the common cases, to make the individual files even simpler
define(`guard_unused_base', `guard_start()
unused_start()
base()
unused_end()
guard_end()')
define(`guard_base', `guard_start()
base()
guard_end()')

divert(0)dnl
