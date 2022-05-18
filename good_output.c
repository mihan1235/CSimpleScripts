//OUTPUT TEXT IN FILE IN ONE STRING 10000 TIMES
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char **argv)
{
	char c='+';
	for (;;) {
		printf("%c",c);
		fflush(stdout);
	}
	return 0;
}
