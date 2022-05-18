#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	int pid;
	pid=fork();
	if (pid==0) {
		execlp("ls", "ls" , "-l",NULL);
	    exit(0);
    }
    wait(NULL);
    printf("Ok\n");
	return 0;
}

