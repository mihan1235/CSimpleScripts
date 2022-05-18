#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#define inp_r 2
#define inp_r_app 3
#define out_r 4
#define out_inp_r 5

struct lst_w {
	char* ptr;
	struct lst_w *next;
};

struct box {
	struct lst_w * f;
	int* p_arr;
};

int r_length_word(char* point) {
	int i=0;
	while (point[i]!='\0') {
		i++;
	}	
	return i;
}

char *ch_size (int before, int after, char *point) {
	char *buffer;
	int i;
	buffer = malloc (after*sizeof(char));
	for (i=0; i!=before; i++) {
		buffer[i] = point[i];
	}	
	free (point);
	point = buffer;
	return point;
}	
	
char* m_word_size(char* point) {
	char *buffer;
	int i=0,n;
	n=r_length_word(point);
	buffer=malloc((n+1)*sizeof(char));	
	while (i<=n) {
		buffer[i]=point[i];
		i++;
	}	
	free(point);
	point=buffer;
	return point;	
}	
 	
int mk_flags(int* flag,char c,short* err_s, int mode,int* p_fl) {
	int tmp=0;
	if ((c=='>')&&(*flag!=inp_r)&&(tmp==0)&&(mode==0)) {
		*flag=inp_r;
		tmp=1;
	} 
	if ((c=='>')&&(*flag==inp_r)&&(tmp!=1)&&(mode==0)) {
		*flag=inp_r_app;
	}
	if ((c=='<')&&(*flag!=inp_r)&&(mode==0)) {
		*flag=out_r;
	}
	if ((c=='<')&&(*flag==inp_r)&&(mode==0)) {
		*flag=out_inp_r;
	}
	if ((mode==0)&&(c=='&')) {
		*flag=1;
	}
	if ((c=='|')&&(*p_fl==1)) {
		(*p_fl)=2;
	}
	if ((c=='|')&&(*p_fl!=2)){
		(*p_fl)=1;
	}
	return 0;
}

char* mk_end_of_str(short* err_s, char* point, int i,char c,int mode) {
	if (c=='\n') {
		*err_s=1;
	}
	if (point!=NULL) {
		point[i]='\0';
		if ((point[0]=='\0')&&(c=='\n')) {
			return NULL;
		}	
		point=m_word_size(point);
	}
	if (mode==1) {
		*err_s=-1;
		 printf("Error : unpair quotes\n");
		 return point;
	}		    
	return point;	
}

int err_ampersand (short* err_s,int* flag){
	char c;
	if (*flag==1) {
		*err_s=-1;
		*flag=0;
		printf("Error : wrong command using '&'\n");
		while ((c=getchar())!='\n') {};
		return 1;
	}
	return 0;		    
}

int good_condition (char c, int* flag, int mode, int *p_fl) {
	short t,t1,t2,t3,t4,t5;
	t1=(*flag==1)&&(c=='&');
	t2=(c=='>')&&((*flag==inp_r)||(*flag==inp_r_app));
	t3=(c=='<')&&((*flag==out_r)||(*flag==out_inp_r));
	t4=((mode==0)&&(c!='"'))||((c!='"')&&(mode==1));
	t5=(c=='|')&&(*p_fl==1);
	t=!(t1||t2||t3||t5)&&t4;
	return t;
}

char* read_word(short* err_s,short* err_f,int* flag,int* p_fl) {
	char* point=NULL;
	int i=0,k=25,mode=0,p=0;
	char c;
	while ((c=getchar())!=EOF) {
		if ((c!=' ')||(mode==1)){
			p=1;
		    if (c=='\n') {
				return mk_end_of_str(err_s,point,i,c,mode);
		    }    					
		    if (i==0) {
			    point=malloc(k*sizeof(char));
		    }
		    if (i==k) {
			    k+=k;
			    point=ch_size(i,k,point);
		    }
		    if (mk_flags(flag,c,err_s,mode,p_fl)==1) {
				return NULL;
			}		
	        if (c=='"') {
			    mode=!mode;
		    }
			
		    if (good_condition(c,flag,mode,p_fl)){
				if (err_ampersand(err_s,flag)==1) {
					return NULL;
				}
				point[i]=c;
				i++;
			}	
	    }
	    else{
			if (p==1) {
				return mk_end_of_str(err_s,point,i,c,mode);
		    }
		}        
	}
	*err_f=1;
	*err_s=1;
	return NULL;
}		

struct lst_w* read_block(short*err_s, short*err_f,int* flag,int* p_fl){								  
	char* buffer;
	struct lst_w* point=NULL;
	if ((buffer=read_word(err_s,err_f,flag,p_fl))!=NULL) {
		if (*err_s==-1) {
			return NULL;
		}	
		point=malloc(sizeof(*point));
	    point->ptr=buffer;
	    point->next=NULL;
	}    
	return point;
}	

void mk_arr_f(int* a, int* flag, int num) {
	 num--;
	 switch (*flag) {
		case inp_r: 
			a[0]=num;
			break;
		case inp_r_app:
			a[1]=num;
			break;
		case out_r:
			a[2]=num;
		    break;
		case out_inp_r:
			a[3]=num;
	}		
}

int* mk_p_fl_arr() {
	int* a;
	int n=15;
	a=malloc(n*sizeof(int));
	a[0]=0;
	a[15]=0;
	return a;
}

void pr_pipe_arr(int* p_arr) {
	int i=0;
	while(p_arr[i]!=0) {
		printf("%i ",p_arr[i]);
		i++;
	}
	if (p_arr[i]==0) {
		printf("0");
	}
	printf("\n");
}

int *ch_size_of_p_arr (int before, int after, int *p_arr) {
	int *buffer;
	int i;
	buffer = malloc (after*sizeof(int));
	for (i=0; i!=before; i++) {
		buffer[i] = p_arr[i];
	}	
	free (p_arr);
	p_arr = buffer;
	return p_arr;
}	

int* ch_size_p_arr(int* tmp,int* n, int* p_arr) {
	if (*tmp>=*n) {
		(*n)+=(*n);
		p_arr=ch_size_of_p_arr(*tmp,*n,p_arr);
	}
	return p_arr;
}

int* mk_arr_pipe(int* p_arr,int* p_fl,int i, int* tmp, int* n){
	if (*p_fl!=0) {
		if (*p_fl==1) {
			p_arr[*tmp]=i;
			(*tmp)++;
		}
		p_arr=ch_size_p_arr(tmp,n,p_arr);
		if (*p_fl==2) {
			p_arr[*tmp]=i-1;
			(*tmp)++;
			p_arr=ch_size_p_arr(tmp,n,p_arr);
			p_arr[*tmp]=i;
			(*tmp)++;
			p_arr=ch_size_p_arr(tmp,n,p_arr);
		}
		p_arr[*tmp]=0;
	}
	*p_fl=0;
	return p_arr;
}

struct box* make_list(short*err_f,int*flag,int*a,int*p_fl,int*p_arr) {
	struct lst_w *point,*extra,*f;
	struct box p;
	struct box* p1;
	p1=&p;
	short* err_s;
	short err_str=0;
	int i=1,tmp=0,n=15;
	err_s=&err_str;
	extra=read_block(err_s,err_f,flag,p_fl);
	if (*err_s==-1){
		return NULL;
	}
	mk_arr_f(a,flag,i);
	p_arr=mk_arr_pipe(p_arr,p_fl,i,&tmp,&n);	
	point=extra;
	f=point;
	while (err_str!=1) {
		extra=read_block(err_s,err_f,flag,p_fl);
		if (*err_s==-1){
			return NULL;
	    }
	    i++;
	    mk_arr_f(a,flag,i);
	    p_arr=mk_arr_pipe(p_arr,p_fl,i,&tmp,&n);	
		point->next=extra;
		point=extra;
	}   
	p.f=f;
	p.p_arr=p_arr;
	return p1;
}			

int count_list(struct lst_w* point) {
	int i=0;
	while (point!=NULL) {
		i++;
		point=point->next;
	}
	return i;
}

void sub_array(int* a, int len) {
	int i=0;
	len--;
	while (i<=len) {
		if (a[i]!=0) {
			a[i]--;
		}
		i++;
	}
}

void sub_arr_p(int* p_arr){
	int i=0;
	while (p_arr[i]!=0) {	
		if (p_arr[i]!=1) {
			p_arr[i]--;
		}
		i++;
	}
}

char** creat_array(struct lst_w* point,int* a,int* p_arr) {
	char** m;
	int n,i=0,tmp=0,i1=0,i2;
	n=count_list(point);
	m=malloc((n+1)*sizeof(point));
	while (point!=NULL) {
		if (point->ptr[0]!='\0') {
			m[i]=point->ptr;
			i++;
			if ((p_arr[i1])==i){
				i1++;
			}
		}
		else {
			tmp++;
			i2=i1;
			while (p_arr[i2]!=0) {
				p_arr[i2]--;
				i2++;
			}
			i1++;
		}
		point=point->next;	
	}
	m[i]=NULL;
	if (tmp==1) {
		sub_array(a,4);
		if ((a[3]-a[0])==2) {
			sub_array(a,4);
		}
	}
	return m;
}
	
void del_all(char **arr, struct lst_w *p)
{
	struct lst_w *buffer;
	while (p!=NULL)
	{
		buffer=p;
		p=p->next;
		free(buffer->ptr);
		free(buffer);
	}
	free(arr);
}

int exec_cd(char** m) {
	char* tmp;
	int p=0;
	if (m[0]!=NULL) {
		tmp=m[0];
		if ((tmp[0]=='c')&&(tmp[1]=='d')&& (tmp[2]=='\0')){
			if (m[1]==NULL) {
				printf("Empty directory\n");
			}
			else {
				p=chdir(m[1]);
				if (p == -1) {
					perror(m[1]);
				}
			}
			return 0;
		}
	}
	return 1;
}	

void kill_zmb(int* flag, int* pid) {
	int p;
	if (*flag!=1) {
		do {
			p=wait(NULL);
		} while (*pid!=p);
	}
	else {
		do {
			p=wait4(-1,NULL,WNOHANG,NULL);
		} while (p>0); 
	}	
}

void mk_null_array(int* a,int length){
	int i=0;
	while (i<=length-1) {
		a[i]=0;
		i++;
	}
}

int f_mode (int* a) {
	if ((a[0]!=0)&&(a[3]==0)) {
		return inp_r;
	}
	if (a[1]!=0) {
		return inp_r_app;
	}
    if (a[2]!=0) {
		return out_r;
	}
	if (a[3]!=0) {
		return out_inp_r;
	}
	return 0;
}

void set_chanel (int fd, char* m, int mode) {
	if (fd<0){
		perror(m);
	}
	else {
		dup2(fd,mode);
	}
}

int exec_cmd(char** m, int* a, int* flag) {
	int pid,fd;
	pid=fork();
	if (pid==-1){
		perror("fork");
		return 1;
	}
	if (pid==0) {
		switch (f_mode(a)) {
			case inp_r:
				fd=open(m[a[0]],O_WRONLY|O_CREAT|O_TRUNC,0666);
				set_chanel(fd,m[a[0]],1);
				m[a[0]]=NULL;
				break;
			case inp_r_app:
				fd=open(m[a[1]],O_WRONLY|O_CREAT|O_APPEND,0666);
				set_chanel(fd,m[a[1]],1);
				m[a[1]]=NULL;
				break;
			case out_r:
				fd=open(m[a[2]],O_RDONLY);
				set_chanel(fd,m[a[2]],0);
				m[a[2]]=NULL;
				break;
			case out_inp_r:
				fd=open(m[a[3]],O_RDONLY);
				set_chanel(fd,m[a[3]],0);
				m[a[3]]=NULL;
				fd=open(m[a[3]-1],O_WRONLY|O_CREAT|O_TRUNC,0666);
				set_chanel(fd,m[a[3]-1],1);
				m[a[3]-1]=NULL;
		}
		execvp(m[0],m);
		perror(m[0]);
		exit(1);	
	}
	kill_zmb(flag,&pid);
	return 0;
}


void print_array(char** m) {
	int i=0;
	while (m[i]!=NULL) {
		printf("[%s] ",m[i]);
		i++;
	}
	printf("\n");
}

int count_pipe_arr(int* p_arr) {
	int i=0;
	while(p_arr[i]!=0) {
		i++;
	}
	return i;
}

char* mk_p_cmd(char**m,char* p,int* p_arr,int number,int place) {
	p=m[p_arr[number]];
	m[p_arr[number]]=NULL;
	return p;
}

void close_pipe(int* fd,int* fd1,int place) {
	if (place!=1) {
		if (place % 2==1) {
			close(fd1[0]);
			close(fd1[1]);
		}
		if (place % 2==0) {
			close(fd[0]);
			close(fd[1]);
		}
	}	
}

int exec_p_cm(char**m,int number,int place,int*fd,int*fd1,int* p_arr) {
	if (place==1) {
		if(fork()==0) {
			close(fd[0]); 
			dup2(fd[1], 1); 
			close(fd[1]); 
			execvp(m[0],m);
			exit(1);
		}	
		return 0;
	}
	if (place==count_pipe_arr(p_arr)+1) {
		if(fork()==0) { 		
			close(fd[1]);
			dup2(fd[0],0);	
			execvp(m[0],m);
			exit(1);
		}
		return 0;
	}
	if(fork()==0) { 
		dup2(fd[0], 0); 
		close(fd[0]); 
		close(fd[1]);
		close(fd1[0]);
		dup2(fd1[1], 1);
		execvp(m[0],m);
		exit(1);
	}
	return 0;
}

int exec_p_cmd(char**m,int number,int place,int*fd,int*fd1,int* p_arr) {
	if (place==1) {
		exec_p_cm(m,number,place,fd,fd1,p_arr);
	}
	if (place!=1) {
		if (place % 2==1) {
			exec_p_cm(m,number,place,fd1,fd,p_arr);
		}
		if (place % 2==0) {
			exec_p_cm(m,number,place,fd,fd1,p_arr);
		}
	}
	return 0;
}

void launch_pipe(int place,int* fd, int* fd1, int* p_arr) {
	if (place!=count_pipe_arr(p_arr)+1) {
		if (place % 2==1) {
			pipe(fd);
		}
		if (place % 2==0) {
			pipe(fd1);
		}
	}	
}

void kill_zmb_p(int place) {
	int i;
	i=1;
	while (i<place+1) {
		wait(NULL);
		i++;
	}
}

int exec_cmd_p(char** m,int* a,int* p_arr,int* flag){
	char** m1;
	m1=m;
	if (p_arr[0]==0) {
		return 0;
	}
	int fd[2];
	int fd1[2];
	char* p=NULL;
	int place=0;
	int number=0;
	while (place!=count_pipe_arr(p_arr)+1) {
		place++;
		launch_pipe(place,fd,fd1,p_arr);
		if (!((place==1)||(place==count_pipe_arr(p_arr)+1))){
			number++;
		}
		if (place!=count_pipe_arr(p_arr)+1) {
			p=mk_p_cmd(m,p,p_arr,number,place);
		}
		
		exec_p_cmd(m1,number,place,fd,fd1,p_arr);		
		close_pipe(fd,fd1,place);
		if (place!=count_pipe_arr(p_arr)+1) {
			m[p_arr[number]]=p;
			m1=m;
			m1=m1+p_arr[number];
		}
	}
	kill_zmb_p(place);
	return 1;
}

int main() {
	struct lst_w *first;
	struct box* p;
	short error=0;
	char** m;
	int a[4];
	int flag_int=0;
	int p_fl=0;
	int* p_arr;
	mk_null_array(a,4);
	while (error!=1) {
		printf("-->");
		p_arr=mk_p_fl_arr();
	    p=make_list(&error,&flag_int,a,&p_fl,p_arr);
	    first=p->f;
	    p_arr=p->p_arr;
	    m=creat_array(first,a,p_arr);
	    if (m[0]!=NULL) {
			if (exec_cd(m)==1) {
				if (exec_cmd_p(m,a,p_arr,&flag_int)==0) {
					exec_cmd(m,a,&flag_int);
				}
			}
		}  
	    flag_int=0;  
	    p_fl=0;
	    free(p_arr);
	    mk_null_array(a,4);  				
	    del_all(m,first);
	  }
	  wait(NULL);
	  return 0;
}
