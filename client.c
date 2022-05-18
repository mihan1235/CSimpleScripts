#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	if (argv[1]==NULL) {
		printf("Usage:  %s [ip-address] ",argv[0]);
		printf("[number of port] \n");
		return 1;
	}
	if (argv[2]==NULL) {
		printf("You have fogotten to input number of port! \n");
		printf("Usage:  %s [ip-address] ",argv[0]);
		printf("[number of port] \n");
		return 1;
	}
	int port;
	struct sockaddr_in addr;
	int sd;
	int tmp;
	char buf[1024];
	port=atoi(argv[2]);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	tmp=inet_aton(argv[1], &(addr.sin_addr));
	if (tmp==0) {
		perror("Unexpected error occurred,while operating with ");
		perror("IP-Address!");
		return 1;
	}
	sd=socket(AF_INET, SOCK_STREAM, 0);
	if (sd == -1) {
		perror("Couldn`t create socket!");
		return 1;
	}
	tmp=connect(sd, (struct sockaddr *)&addr,sizeof(addr));
	if (tmp==-1) {
		perror("Couldn`t connect to server!");
        close(sd);
        return 1;
	}
	printf("You have sucsessfully connected to server\n");
	for(;;) { 
		fd_set readfds;		
		FD_ZERO(&readfds); 
		FD_SET(sd, &readfds);
		FD_SET(0,&readfds);
		tmp = select(sd+1, &readfds, NULL, NULL, NULL);	
		if(tmp < 1) {
			perror("Error while selecting \n");
			return 1;
		}
		if(FD_ISSET(0, &readfds)) {
			tmp=read(0,buf,sizeof(buf)-1);
			buf[tmp]='\0';
			write(sd,buf,strlen(buf)+1);
		}
		if(FD_ISSET(sd, &readfds)) {
			tmp=read(sd,buf,sizeof(buf)-1);
			buf[tmp]='\0';
			if (tmp==0) {
				printf("Conection with server is lost!\n");
				FD_CLR(sd,&readfds);
				FD_CLR(0,&readfds);
				shutdown(sd,SHUT_RDWR);
				close(sd);
				return 1;
			}
			write(0,buf,strlen(buf)+1);
			printf("\n");
		}
	}
	return 0;
}

