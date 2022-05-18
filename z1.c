#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
/*Задача 1.
Из стандартного потока ввода читается текст до ситуации конец файла. 
Надо вывести 10 самых часто встречаемых слов в формате: слово — кол-во 
* встреченных раз. Слова состоят только из больших и маленьких латинских
*  букв и цифр. Остальные символы считаются разделительными.

Проверяет: ./prog1 < prog1.c и она должна напечатать 10 самых часто
*  используемых слов из исходника вашей программы. 

В другом варианте нужно было напечатать все слова максимальной длины.
*/

struct block {
		char*p;
		int n;
		struct block* next;
};

int main()
{	
	char*p;
	char*buf;
	char c;
	int i=0;
	int k=2;
	struct block* point=NULL;
	struct block* point1;
	struct block* point2;
	int ch=0,tmp=0;
	p=malloc(k*sizeof(char));
	while ((c=getchar())!=EOF) {
		if (c==' ') {
			while ((c=getchar())==' ') {}
		}
		while (((c>='a')&&(c<='z'))||((c>='A')&&(c<='Z'))){
			p[i]=c;
			i++;
			c=getchar();
			if (i==k) {
				k=+k;
				p=realloc(p,k*sizeof(char));
			}
		}
		p[i]='\0';
		i=0;
		if (p[0]!='\0') {
			if (point==NULL) {
				point=malloc(sizeof(*point));
				point->p=p;
				point->n=1;
				point->next=NULL;
				p=NULL;
				point2=point;
			}
			else {
				point1=point;
				while (point1!=NULL) {
					if (strcmp(point1->p,p)==0) {
						point1->n++;
						break;
					}
					point1=point1->next;
				}
				if (point1==NULL) {
					point1=point2;
					point1->next=malloc(sizeof(*point1));
					point1=point1->next;
					point1->p=p;
					point1->n=1;
					point1->next=NULL;
					p=NULL;
					point2=point2->next;
				}
			}
			p=malloc(k*sizeof(char));
		}
	}
	free(p);
	do {
		ch=0;
		point1=point;
		point2=point1->next;
		while (point2!=NULL) {
			if (point1->n<point2->n) {
				tmp=point1->n;
				point1->n=point2->n;
				point2->n=tmp;
				buf=point1->p;
				point1->p=point2->p;
				point2->p=buf;
				ch=1;
			}
			point1=point1->next;
			point2=point2->next;
		}
	}while (ch==1);
	printf("\n");
	point1=point;
	i=0;
	while (point1!=NULL) {
		printf("[%s %i] ",point1->p,point1->n);
		point1=point1->next;
		i++;
		if (i==10) {
			break;
		}
	}
	printf("\n");
	point1=point;
	while (point1!=NULL) {
		free(point1->p);
		point2=point1;
		point1=point1->next;
		free(point2);
	}
	return 0;
}

