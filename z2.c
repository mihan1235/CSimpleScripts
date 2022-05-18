#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
/* Задача 2.
В аргументах командной строки передаются имя файла 1, имя файла 2,
*  а также имя и аргументы для вызова программы. Надо запустить эту 
* программу, подав ей на вход содержимое файла 1 и перенаправив ее вывод
*  в файл 2. Родительский процесс должен подождать 1 с, и приостановить 
* выполняемую программу с помощью сигнала SIGSTOP, подождать еще секунду
*  и продолжить выполнение с помощью сигнала SIGCONT и так далее.  
*  Родительский процесс должен немедленно завершится, как только 
* завершится дочерний процесс (это можно делать разными способами: с
*  помощью комбинации wait/alarm, обработки сигнала SIGCHLD, третий не 
* помню, как-то через вспомогательный процесс).

Проверяет: надо самому создать два fifo-файла 
mkfifo fff1
mkfifo fff2
запустить ./prog1 fff1 fff2 grep int

а дальше начиналась какая-то магия... А.В. открывал еще 2 консоли. 
* Делал так что в fff1 бесконечно печатался текст нашего исходника 
* (наверно каким-нибудь yes, я не успела заметить), в другой консоли
*  он выводил fff2. Так вот там должны были раз в секунду печататься 
* все строки в которых есть int (все сразу, разумеется). Когда он 
* прибивал это дело, программа должна была сразу завершится.

В другом варианте нужно было посылать SIGSTOP и SIGCONT дочернему
*  процессу по нажатию Ctrl-C (то есть, по первому нажатию послать
* SIGSTOP, по второму — SIGCONT, и т.д.), при нажатии Ctrl-C с 
* интервалом менее одно секунды послать дочернему процессу SIGKILL 
* и немедленно завершить родительский процесс.
*/

int main(int argc, char **argv)
{
	int fd,fd1;
	int i=0,i1=0;
	long int sum=0;
	char** m;
	int pid,pid1;
	if (argv[1]==NULL) {
		printf("Null parameter!!\n");
		return 1;
	}
	fd=open(argv[1],O_RDONLY);
	if (fd<0) {
		perror(argv[1]);
		return 1;
	}
	fd1=open(argv[2],O_WRONLY|O_APPEND,0666);
	if (fd1<0) {
		perror(argv[2]);
		return 1;
	}
	while (i<argc) {
		sum=sum+strlen(argv[i]);
		i++;
	}
	m=malloc(sum*sizeof(char)+argc);
	i=3;
    while (i<argc) {
		m[i1]=argv[i];
		i1++;
		i++;
	}
	m[i1]=NULL;
	pid=fork();
	if (pid==-1) {
		perror("fork()");
	}
	if (pid==0) {
		dup2(fd,0);
		dup2(fd1,1);
		execvp(m[0],m);
		perror(m[0]);
		exit(1);
	}
	i=0;
	for (;;) {
		pid1=wait4(-1,NULL,WNOHANG,NULL);
		if (pid==pid1) {
			break;
		}
		sleep(1);
		if (i==0) {
			kill(pid,SIGSTOP);
			i=!i;
		}
		else {
			kill(pid,SIGCONT);
			i=!i;
		}
	}	
	return 0;
}

