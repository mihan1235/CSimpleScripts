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
 	
int mk_flags(int* flag,char c,short* err_s, int mode) {
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

int good_condition (char c, int* flag, int mode) {
	short t,t1,t2,t3,t4;
	t1=(*flag==1)&&(c=='&');
	t2=(c=='>')&&((*flag==inp_r)||(*flag==inp_r_app));
	t3=(c=='<')&&((*flag==out_r)||(*flag==out_inp_r));
	t4=((mode==0)&&(c!='"'))||((c!='"')&&(mode==1));
	t=!(t1||t2||t3)&&t4;
	return t;
}

char* read_word(short* err_s,short* err_f,int* flag) {
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
		    if (mk_flags(flag,c,err_s,mode)==1) {
				return NULL;
			}	
		    if ((mode==0)&&(c=='&')) {
				*flag=1;
			}	
	        if (c=='"') {
			    mode=!mode;
		    }
		    if (good_condition(c,flag,mode)){
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

struct lst_w* read_block(short*err_s, short*err_f,int* flag){								  
	char* buffer;
	struct lst_w* point=NULL;
	if ((buffer=read_word(err_s,err_f,flag))!=NULL) {
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
	  
struct lst_w* make_list(short* err_f,int* flag,int* a) {
	struct lst_w *point,*extra,*f;
	short* err_s;
	short err_str=0;
	int i=1;
	err_s=&err_str;
	extra=read_block(err_s,err_f,flag);
	if (*err_s==-1){
		return NULL;
	}
	mk_arr_f(a,flag,i);	
	point=extra;
	f=point;
	while (err_str!=1) {
		extra=read_block(err_s,err_f,flag);
		if (*err_s==-1){
			return NULL;
	    }
	    i++;
	    mk_arr_f(a,flag,i);
		point->next=extra;
		point=extra;
	}   
	return f;
}			

int count_list(struct lst_w* point) {
	int i=0;
	while (point!=NULL) {
		i++;
		point=point->next;
	}
	return i;
}

void sub_array(int* a, int num, int len) {
	int i=0;
	len--;
	while (i<=len) {
		if (a[i]!=0) {
			a[i]--;
		}
		i++;
	}
}

char** creat_array(struct lst_w* point,int* a) {
	char** m;
	int n,i=0,tmp=0;
	n=count_list(point);
	m=malloc((n+1)*sizeof(point));
	while (point!=NULL) {
		if (point->ptr[0]!='\0') {
			m[i]=point->ptr;
			i++;
		}
		else {
			tmp=1;
		}
		point=point->next;	
	}
	m[i]=NULL;
	if (tmp==1) {
		sub_array(a,1,4);
		if ((a[0]!=0)&&(a[3]!=0)) {
			sub_array(a,1,4);
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

void print_array(char** m) {
	int i=0;
	while (m[i]!=NULL) {
		printf("[%s]\n",m[i]);
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

int main() {
	struct lst_w *first;
	short error=0;
	char** m;
	int a[4];
	int flag_int;
	mk_null_array(a,4);
	while (error!=1) {
		printf("-->");
	    first=make_list(&error,&flag_int,a);
	    m=creat_array(first,a);
	    if (m[0]!=NULL) {
			if (exec_cd(m)==1) {
				exec_cmd(m,a,&flag_int);
			}
		}  
	    flag_int=0;
	    mk_null_array(a,4);  				
	    del_all(m,first);
	  }
	  wait(NULL);
	  return 0;
}
