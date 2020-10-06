#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "threads.h"

int main ()
{
	srand(time(NULL));
	int status;

	printf("Start threads_main...\n");
	status = threads_main();
	if (status != 0)
		printf("main threads_main error\n");
	return SUCCESS;
}