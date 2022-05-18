/*сортировка чисел в файле по возрастанию*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int chg_element(int fd,int* elem1,int* elem2){
	lseek(fd,(-2)*sizeof(int),1);
	write(fd,elem2,sizeof(int));
	write(fd,elem1,sizeof(int));
    return 0;
}

int cmp_element(int* elem1, int* elem2){
	if (*elem1>*elem2) {
		return 1;
	}
	else {
		return 0;
	}
	return 0;
}

int read_pair (int fd,int* elem1, int* elem2){
	int tmp1,tmp2;
	tmp1=read(fd,elem1,sizeof(int));
	tmp2=read(fd,elem2,sizeof(int));
	if ((tmp1==0)||(tmp2==0)) {
		return 0;
	}
	return 1;
}

int sort_file (int fd){
	int elem1,elem2;
	int ch;
	int i=0;
	do {
		lseek(fd,0,0);
		ch=0;
		while (read_pair(fd,&elem1,&elem2)!=0) {
			if (cmp_element(&elem1,&elem2)){
				chg_element(fd,&elem1,&elem2);
				ch=1;
			}
			lseek(fd,(-1)*sizeof(int),1);
		}
		i++;
		printf("|");
		fflush(stdout);
	}while (ch==1);
	printf("\n %i \n",i);
  return 0;  
}

int main (int argc, char** argv) {
	int fd;	
	int i,n;
	int b;
	if (argv[1]==NULL) {
		printf("Usage:  %s [name of file ] [max number]\n",argv[0]);
		return 1;
	}
	if (argv[2]==NULL) {
		printf("Usage:  %s [name of file ] [max number]\n",argv[0]);
		return 1;
	}
	fd=open(argv[1],O_RDWR|O_CREAT|O_TRUNC,0666);
	srand(getpid());
	n=atoi(argv[2]);
	for (i=0; i <n; i++) {
		b=random() % 1000;
		write(fd,&b,sizeof(b));
	}
	sort_file(fd);
	close(fd);
	fd=open(argv[1],O_RDONLY);
	for (i=0; i <n; i++) {
		read(fd,&b,sizeof(b));
		printf("%i ",b);
	}
	printf("\n");
	close(fd);
	unlink(argv[1]);
	return 0;
}
