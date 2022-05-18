#include <stdio.h>

void address(short*p) {
	*p=0;
}

int main()
{
	short k=1;
	address(&k);
	printf("number: %i \n",k);
	printf("%i",sizeof(char));
	return 0;
}

