#include <assert.h>
#include "../sassert.h"


#include <stdio.h>



static_assert(1 == 1, "This should never fail");


int main(int argc, char **argv)
{
	static_assert(0 == 0, "this should also never fail");
	static_assert(1 == 2, "This should always fail");
	return 0;
}

