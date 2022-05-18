#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

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
 	
char* read_word(short* err_s,short* err_f,int* flag) {
	char* point=NULL;
	int i=0,k=25,mode=0,p=0;
	char c;
	while ((c=getchar())!=EOF) {
		if ((c!=' ')||(mode==1)){
			p=1;
		    if (c=='\n') {
				*err_s=1;
			    if (point!=NULL) {
					point[i]='\0';
					if (point[0]=='\0') {
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
		    if (i==0) {
			    point=malloc(k*sizeof(char));
		    }
		    if (i==k) {
			    k+=k;
			    point=ch_size(i,k,point);
		    }	
		    if ((mode==0)&&(c=='&')) {
				*flag=1;
			}	
	        if (c=='"') {
			    mode=!mode;
		    }
		    if (!((*flag==1)&&(c=='&'))){
				if (((mode==0)&&(c!='"'))||((c!='"')&&(mode==1))){
					if (*flag==1) {
						*err_s=-1;
						*flag=0;
						printf("Error : wrong command using '&'\n");
						while ((c=getchar())!='\n') {};
						return NULL;
	                }		    
					point[i]=c;
					i++;
				}
			}	
	    }
	    else{
			if (p==1) {
				point[i]='\0';
		        point=m_word_size(point);
		        return point;
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
struct lst_w* make_list(short* err_f,int* flag) {
	struct lst_w *point,*extra,*f;
	short* err_s;
	short err_str=0;
	err_s=&err_str;
	extra=read_block(err_s,err_f,flag);
	if (*err_s==-1){
		return NULL;
	}	
	point=extra;
	f=point;
	while (err_str!=1) {
		extra=read_block(err_s,err_f,flag);
		if (*err_s==-1){
			return NULL;
	    }	
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

char** creat_array(struct lst_w* point) {
	char** m;
	int n,i=0;
	n=count_list(point);
	m=malloc((n+1)*sizeof(point));
	while (point!=NULL) {
		m[i]=point->ptr;
		point=point->next;
		i++;
	}
	m[n]=NULL;
	return m;
}
	
void print_array(char** m) {
	int i=0;
	while (m[i]!=NULL) {
		printf("[%s]\n",m[i]);
		i++;
	}
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

int main() {
	struct lst_w *first;
	short error=0;
	short* err_f;
	char** m;
	char* tmp;
	int flag_int;
	int* flag=&flag_int;
	int pid,p;
	err_f=&error;
	while (error!=1) {
		printf("-->");
	    first=make_list(err_f,flag);
	    m=creat_array(first);
	    if (m[0]!=NULL) {
			tmp=m[0];
			if ((tmp[0]=='c')&&(tmp[1]=='d')&& (tmp[2]=='\0')){
				if (m[1]==NULL) {
					printf("Empty directory\n");
				}
				else {
					pid=chdir(m[1]);
					if (pid == -1) {
						perror(m[1]);
					}
				}
			}
			else {
				pid=fork();
			    if (pid==-1){
				    perror("fork");
				    return 1;
		        }
		        if (pid==0) {
					execvp(m[0],m);
				    perror(m[0]);
			        exit(1);	
		        }
		        if (flag_int!=1) {
					do {
						p=wait(NULL);
					} while (pid!=p);
				}
				else {
					do {
						p=wait4(-1,NULL,WNOHANG,NULL);
					} while (p>0); 
				}
			}
		}  
	    flag_int=0;    				
	    del_all(m,first);
	  }
	  wait(NULL);
	  return 0;
}
