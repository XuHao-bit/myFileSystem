#include <stdio.h>
#include <string.h>

#include "Execute.h"

int main()
{
	char lineInput[512];

	while (1)
	{
		gets_s(lineInput);
		
		if (strcmp("shutdown", lineInput) == 0)
		{
			printf("Exit!\n");
			break;
		}

		Execute(lineInput);
	}

	return 0;
}