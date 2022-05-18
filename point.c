#include <stdio.h>
#include <stdlib.h>

struct box {
	int* f;
	int i;
};

struct box* mk_f(int* f, int*i) {
	struct box l;
	struct box* l1;
	l1=&l;
	int* g;
	g=malloc(2*sizeof(int));
	f[0]=8;
	g[0]=f[0];
	g[1]=6;
	free(f);
	f=g;
	*i=9;
	l.f=f;
	l.i=90;
	return l1;
}

int main() {
	int* f=NULL;
	int i;
	struct box* l;
	f=malloc(sizeof(int));
	l=mk_f(f,&i);
	f=l->f;
	i=l->i;
	printf("%i %i %i\n",f[0],f[1],i);
	free(f);
	return 0;
}

