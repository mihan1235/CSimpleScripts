#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
 

int main(int argc, char **argv)
{
	char buf[2]="ab";
	int fd=creat("/a.txt",0777);
	write(fd,buf,2);
	close(fd);
	fd=open("/a.txt",O_RDONLY);
	fork();
	read(fd,buf,1);
	printf("%c",buf[0]);
	wait(NULL);
	return 0;
}

