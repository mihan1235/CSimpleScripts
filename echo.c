#include <stdio.h>

int main(int argc, char **argv)
{
	char **a;
	*a=malloc(sizeof(char));
	a[0]="firefox";
	execvp("firefox");	
	return 0;
}

