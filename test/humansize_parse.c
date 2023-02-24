#include <stdio.h>
#include "../humansize.h"



int main(int argc, char **argv)
{
	int ret;
	double res;
	char *s = argv[1];
	
	ret = humansize_parse(s, 0, &res);
	printf("For string '%s', base 0: return %d, value %f\n", s,ret,  res);
	ret = humansize_parse(s, 2, &res);
	printf("For string '%s', base 2: return %d, value %f\n", s, ret, res);
	ret = humansize_parse(s, 10, &res);
	printf("For string '%s', base 10: return %d, value %f\n", s,ret,  res);

}

