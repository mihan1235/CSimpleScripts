# include <stdlib.h>
# include <stdio.h>
# include <signal.h>
# include <unistd.h>
#include <unistd.h>
char c;

void held(int n) {
	printf("+");
	fflush(stdout);
}

int main(int argc, char **argv)
{
	signal(SIGALRM,held);
	for (;;) {
		alarm(1);
		pause();
	}
	
	return 0;
}

