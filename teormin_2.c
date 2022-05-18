#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
 

int main(int argc, char **argv)
{
	int fd[2],buf;
	pipe(fd);
	buf=1;
	if (fork()){
		write(fd[1],&buf,sizeof(int));
	}
	else {
		while (read(fd[0],&buf,sizeof(int))){
			buf++;
		}
		printf("%d\n",
		buf);
	}
	return 0;
}

