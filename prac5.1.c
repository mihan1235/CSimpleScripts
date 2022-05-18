#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>


char c;

char* mk_word(void) {
	int k=25;
	int i=0;
	int mode=0;
	char* p=malloc(k*sizeof(char));
	if (c==' ') {
		while ((c=getchar())==' '){};
	}
	if (c=='"') {
		mode=!mode;
		c=getchar();
	}
	while (((c!=' ')&&(c!='\n')&&(c!='|'))||(mode==1)) {
		if (i==(k-1)) {
			k=+k;
			p=realloc(p,k*sizeof(char));
		}
		p[i]=c;
		i++;
		c=getchar();
		if (c=='"') {
			mode=!mode;
			c=getchar();
		}
	}
	p[i]='\0';
	return p;
}

void print_cmd(char** m) {
	int i=0;
	while (m[i]!=NULL) {
		printf("%s ",m[i]);
		i++;
	}
	printf("\n");
}

int length_cmd(char** m) {
	int i=0;
	while (m[i]!=NULL) {
		i++;
	}
	return i;
}

int length_string(char* point) {
	int i=0;
	while (point[i]!='\0') {
		i++;
	}	
	return i;
}

void free_cmd(char** m) {
	int i=0;
	while (m[i]!=NULL) {
		free(m[i]);
		i++;
	}	
	free(m);
}

char** mk_cmd(void) {
	int sum=5;
	char*p;	
	char** m;
	char**m1;
	int tmp=0;
	int i=0;
	int i1=0;
	m=malloc(sum*sizeof(char*));
	while ((c!='\n')&&(c!='|')) {
		p=mk_word();
		tmp=tmp+length_string(p)+1;
		if (tmp>sum) {
			sum=sum+sum;
			m1=malloc(sum*sizeof(char*));
			i1=0;
			while (m[i1]!=NULL) {
				m1[i1]=m[i1];
				i1++;
			}
			m1[i1]=NULL;
			m=NULL;
			m=m1;
		}
		m[i]=p;
		i++;
		m[i]=NULL;
	}
	m[i]=NULL;
	return m;
}

void print_arr(char*** m) {
	int i=0;
	while (m[i]!=NULL) {
		print_cmd(m[i]);
		i++;
	}
	printf("\n");
}

void free_arr( char***m) {
	int i=0;
	while (m[i]!=NULL) {
		free_cmd(m[i]);
		i++;
	}	
	free(m);
}

int main() {
	char** m;
	char*** m1;
	char*** m2;
	int sum=52;
	int tmp=0;
	int i=0;
	int i1=0;
	printf("--> ");
	m1=malloc(sum*sizeof(char**));
	while ((c=getchar())!=EOF){
		m=mk_cmd();
		tmp++;
		//print_cmd(m);
		if (tmp==(sum-1)) {
			sum=sum+sum;
			m2=malloc(sum*sizeof(char**));
			i1=0;
			while (m1[i1]!=NULL) {
				m2[i1]=m1[i1];
				i1++;
			}
			m2[i1]=NULL;
			m1=NULL;
			m1=m2;
		}
		m1[i]=m;
		i++;
		m1[i]=NULL;
		//free_cmd(m);
		if (c=='\n') {
			tmp=0;
			sum=52;
			i=0;
			i1=0;
			print_arr(m1);
			free_arr(m1);
			m1=malloc(sum*sizeof(char**));
			printf("--> ");
		}
	}
	free(m1);
	printf("\n");
	return 0;
}
