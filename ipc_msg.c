/*Участвуют три процесса: основной процесс и процессы A и B. Основной 
 * процесс считывает из стандартного ввода текстовую строку. Если она 
 * начинается на букву A, то эта строка посылается процессу A, если на
 *  B — то процессу B, если на Q — то обоим процессам (в этом случае 
 * основной процесс ждет некоторое время, затем удаляет очередь 
 * сообщений и завершается). Процессы A и B считывают из очереди 
 * адресуемые им сообщения и распечатывают их на экране. Если пришедшее 
 *  сообщение начинается с буквы Q, то процесс завершается.
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>


struct Message {
	long mtype;
	char Data[256];
}; 

int main(int argc, char **argv) {
	key_t key;
	int msgid;
	char str[256];
	struct Message message;
	key = ftok("/usr/mash",'s');
	msgid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL);
	for(;;) {
		scanf("%256s",str);
		strcpy(message.Data, str);
		switch(str[0]){
			case 'a':
			case 'A':
				message.mtype = 1;
				msgsnd(msgid,&message,strlen(str)+1,0);
				break;
			case 'b':
			case 'B':
				message.mtype = 2;
				msgsnd(msgid,&message,strlen(str)+1,0);
				break;
			case 'q':
			case 'Q':
				message.mtype = 1;
				msgsnd(msgid,&message,strlen(str)+1,0);
				message.mtype = 2;
				msgsnd(msgid,&message,strlen(str)+1,0);
				sleep(2);
				msgctl(msgid, IPC_RMID, NULL);
				exit(0);
			default:
				break;
		}
	}
	return 0;
}
