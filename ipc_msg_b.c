#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>

struct Message 	{
	long mtype;
	char Data[256];
}; 

int main(int argc, char **argv) {
	key_t key;
	int msgid;
	struct Message message;
	key = ftok("/usr/mash",'s');
	msgid = msgget(key, 0666);
	for(;;) {
		msgrcv(msgid,&message, 256, 2, 0);
		printf("%s\n", message.Data);
		fflush(stdout);
		if(message.Data[0] == 'q' || message.Data[0] == 'Q') {
			break;
		}
	}
	return 0;
}
