# include <stdlib.h>
# include <stdio.h>
# include <signal.h>
# include <unistd.h>

void hdl1(int);
char c='+';

void hdl2(int n)
{
	signal(SIGINT,hdl1);
}

void hdl1(int n)
{
	if (c=='+') {
		c='-';
	}
	else {
		c='+';
	}
	signal(SIGINT,SIG_DFL);
	signal(SIGALRM,hdl2);
	alarm(2);
}

int main ()
{
	signal(SIGINT,hdl1);
	while (1) {
		sleep(1);
		printf("%c ",c);
		fflush(stdout);
	}
	return 0;
}
