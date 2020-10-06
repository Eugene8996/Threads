#include <stdio.h>

#include "threads.h"

int main ()
{
	int status;

	printf("Start threads_main...\n");
	status = threads_main();
	if (status != 0)
		printf("main threads_main error\n");
	return SUCCESS;
}