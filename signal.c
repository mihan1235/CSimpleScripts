#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
int m=0;
	void hdl() {
		
		if (m<25) {
			printf(" %i ",m);
			printf("no\n");
			m++;
		}
		else {
			exit(0);
		}			
	
}

int main () {
	signal(SIGINT,hdl);
	while (1) {
		
	}
	return 0;	
}
