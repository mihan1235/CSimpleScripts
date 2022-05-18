#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

/*Задача 3. 
Программа через аргументы командной строки получает имена и аргументы 
* для запуска программ ( 2 имени!), разделенные параметром «---». Надо
*  запустить конвейер из этих команд, причем на ввод следующей команде 
* пойдут только те строки вывода предыдущей, которые начинаются с
*  символов пробела и табуляции.

Проверяет: ./prog1 cat prog1.c --- grep int
соответственно программа должна напечатать те строки исходника вашей 
* программы, в которых содержится слово int и которые начинаются с 
* символов пробела или табуляции.

В другом варианте эта задача была такая же, но на ввод следующей команде
*  нужно было послать только нечетные строки вывода предыдущей.
*/

void print_array(char** m) {
	int i=0;
	while (m[i]!=NULL) {
		printf("[%s] ",m[i]);
		i++;
	}
	printf("\n");
}

 int main(int argc, char **argv) {
	int i=0,i1=0;
	long int sum=0;
	char** m;
	char** m1;
	char* s="---";
	char* s1="int";
	char s2[4];
	int fd[2];
	int fd1[2];
	int pid;
	char c;
	if (argv[1]==NULL) {
		printf("Null parameter!!\n");
		return 1;
	}
	i=1;
	while (i<argc) {
		if (strcmp(s,argv[i])==0) {
			break;
		}
		i++;
	}
	i1=1;
	while (i1<i) {
		sum=sum+strlen(argv[i1]);
		i1++;
	}
	m=malloc(sum*sizeof(char)+argc);
	i1=1;
    while (i1<i) {
		m[i1-1]=argv[i1];
		i1++;
	}
	m[i1-1]=NULL;
	sum=0;
	while (i1<argc) {
		sum=sum+strlen(argv[i1]);
		i1++;
	}
	m1=malloc(sum*sizeof(char)+argc);
	i1=i+1;
	i=0;
    while (i1<argc) {
		m1[i]=argv[i1];
		i++;
		i1++;
	}
	m1[i]=NULL;
	print_array(m);
	print_array(m1);
	pipe(fd);
	pid=fork();
	if (pid==0) {
		dup2(fd[1],1);
		close(fd[0]);
		close(fd[1]);
		execvp(m[0],m);
		perror(m[0]);
		return 1;
	}
	
	pipe(fd1);
	pid=fork();
	if (pid==0) {
		dup2(fd[0],0);
		close(fd[1]);
		close(fd[0]);
		dup2(fd1[1],1);
		close(fd1[0]);
		close(fd1[1]);
		execvp(m1[0],m1);
		perror(m1[0]);
		return 1;
	}
	
	
	pid=fork();
	if (pid==0) {
		dup2(fd1[0],0);
		close(fd[1]);
		close(fd[0]);
		close(fd1[0]);
    	close(fd1[1]);
		while ((c=getchar())!=EOF) {
			if ((c!=' ')&&(c!='\t')){
				while ((c=getchar())!='\n'){}
				c=getchar();
			}
			if ((c==' ')||(c=='\t')) {
				i=0;
				while (i<3) {
					s2[i]=getchar();
					i++;
				}
				s2[i]='\0';
				
				if (strcmp(s2,s1)==0) {
					printf("%c",c);
					printf("%s",s2);
					while ((c=getchar())!='\n'){
						printf("%c",c);
					}
					printf("%c",c);
				}
			}
		}
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
	
