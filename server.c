#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

const char err_full[]="The server is full, sorry!";
const char num[]="num\n";
const char un_cmd[]="Unknown command, use command 'help'";
const char help[]="You can use commands: num,help. For more imformation"
" use man 'cmd'";
const char h[]="help\n";
const char man_h[]="man help\n";
const char man_help[]="This command helps you to understand what to do";
const char man_num[]="This command shows the number of players";
const char man_n[]="man num\n";

void reverse(char s[])
 {
     int i, j;
     char c;
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }

void itoa(int n, char s[])
 {
     int i, sign;
     if ((sign = n) < 0) {
         n = -n;   
     }      
	 i = 0;
     do {      
         s[i] = n % 10 + '0';  
         i++;
     } while ((n/10) > 0);   
     if (sign < 0) {
         s[i] = '-';
         i++;
     }
     s[i] = '\0';
     reverse(s);
 }

int main(int argc, char **argv)
{
	if (argv[1]==NULL) {
		printf("Usage:  %s [number of port] ",argv[0]);
		printf("[number of players] \n");
		return 1;
	}
	if (argv[2]==NULL) {
		printf("You have fogotten to input number of players! \n");
		printf("Usage:  %s [number of port] ",argv[0]);
		printf("[number of players] \n");
		return 1;
	}
	int port;
	struct sockaddr_in addr;
	int sd;
	int tmp;
	int max_players=atoi(argv[2]);
	int number_of_players=0;
	int max_d;
	int pl_fd[max_players];
	int pl_state[max_players];
	int i=0;
	int cls;
	char buf[1024];
	while (i<max_players) {
		pl_state[i]=0;
		i++;
	}
	i=0;
	port=atoi(argv[1]);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
	sd=socket(AF_INET, SOCK_STREAM, 0);
	if (sd == -1) {
		perror("Couldn`t create socket \n");
		return 1;
	}
	int opt = 1;
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	tmp=bind(sd,(struct sockaddr *) &addr,sizeof(addr));
	if (tmp==-1) {
		perror(" Unexpected error occurred,while binding ");
		close(sd);
		return 1;
	}
	tmp=listen(sd,5);
	if (tmp==-1) {
		perror(" Unexpected error occurred,while listening port ");
		close(sd);
		return 1;
	}
	max_d=sd;
	for(;;) { 
		fd_set readfds;		
		FD_ZERO(&readfds); 
		FD_SET(sd, &readfds);
		for(i=0;i<max_players;i++){
			if (pl_state[i]==1) {
			    FD_SET(pl_fd[i],&readfds);
			    if (pl_fd[i]>max_d) {
                    max_d=pl_fd[i];
                }
			}
		}
		tmp = select(max_d+1, &readfds, NULL, NULL, NULL);	
		if(tmp < 1) {
			perror("Error while selecting \n");
			return 1;
		}
		if(FD_ISSET(sd, &readfds)) {
			cls=accept(sd,0,0);
			if (cls<0) {
				perror("Error accepting \n");
				close(sd);
				return 1;
			}
			if (number_of_players<max_players) {
				i=0;
				while (i<max_players) {
					if (pl_state[i]==0) {
						break;
					}
					i++;
				}
				pl_fd[i]=cls;
				pl_state[i]=1;
				number_of_players++;
			}
			else {
				write(cls,err_full,strlen(err_full)+1);
				shutdown(cls,SHUT_RDWR);
				close(cls);
			}
		}
		//Operation with clients
		for(i=0;i<max_players;i++){
			if (pl_state[i]==1) {
				if(FD_ISSET(pl_fd[i], &readfds)) {
					tmp=read(pl_fd[i],buf,sizeof(buf)-1);
					buf[tmp]='\0';
					if (tmp==0) {
						printf("Conection with client with number ");
						printf("%i is lost. \n",i+1);
						printf("This client was kicked \n");
						FD_CLR(pl_fd[i],&readfds);
						pl_state[i]=0;
						shutdown(pl_fd[i],SHUT_RDWR);
						close(pl_fd[i]);
						number_of_players--;
					}
					if (buf[tmp-1]=='\n') {
						buf[tmp-1]='\0';
					}
					tmp=-1;
					if ((tmp=strcmp(h,buf))==0) {
						write(pl_fd[i],&help,strlen(help)+1);
					}
					if ((tmp=strcmp(num,buf))==0){
						itoa(number_of_players,buf);
						write(pl_fd[i],buf,strlen(buf)+1);                                                                                                                      
					}
					if ((tmp=strcmp(man_h,buf))==0) {
						write(pl_fd[i],&man_help,strlen(man_help)+1);
					}
					if ((tmp=strcmp(man_n,buf))==0) {
						write(pl_fd[i],&man_num,strlen(man_num+1));
					}					
				}
			}
		}
		//Operation with clients ends
	}
	return 0;
}

