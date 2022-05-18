#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
	
	int fd[2];
	int fd1[2];
	//first channel
	pipe(fd);
	if(fork()==0) {
		close(fd[0]); 
		dup2(fd[1], 1); 
		close(fd[1]); 
		execlp("ls", "ls", "-l", NULL);
		perror("ls");
		exit(1);
	}	
	pipe(fd1);
	if(fork()==0) { 
		dup2(fd[0], 0); 
		close(fd[0]); 
		close(fd[1]);
		close(fd1[0]);
		dup2(fd1[1], 1); 
		execlp("grep", "grep", "^d", NULL);
		perror("grep");
		exit(1);
	}
	if(fork()==0) { 	
		close(fd[1]); 	
		close(fd[0]); 	
		close(fd1[1]);
		dup2(fd1[0],0);	
		execlp("cut", "cut", "-f1", "-d", " ", NULL);	
		perror("grep");
		exit(1);
	}

	close(fd[0]);
	close(fd[1]);
	close(fd1[0]); 
	close(fd1[1]);
	wait(NULL); 
	wait(NULL);
	wait(NULL);
	return 0;
}
