#include <stdio.h>

int main()
{
	int p;
	p=fork();
	if (p==0) {
		printf("Red \n");
		exit(0);
	}
	else {
		printf("green\n");
	}
	printf("Blue\n");		
	return 0;
}

