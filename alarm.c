#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

void hdl1(int n) {
	if (alarm(0)!=0) {
		exit(0);
	}
}

void hdl2 (int n) {
	
	alarm(2);
	signal(SIGALRM,hdl1);
}


int main(){
	signal(SIGINT,hdl2);
	while(1) {
		pause();
	}
	return 0;
}

