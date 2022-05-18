#include <stdio.h>
#include <stdlib.h>


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
 	
char* read_word(short* err_s,short* err_f) {
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
	        if (c=='"') {
			    mode=!mode;
		    }	
	        if (((mode==0)&&(c!='"'))||((c!='"')&&(mode==1))){
			    point[i]=c;
		        i++;
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

struct lst_w* read_block(short*err_s, short*err_f){								  
	char* buffer;
	struct lst_w* point=NULL;
	if ((buffer=read_word(err_s,err_f))!=NULL) {
		if (*err_s==-1) {
			return NULL;
		}	
		point=malloc(sizeof(*point));
	    point->ptr=buffer;
	    point->next=NULL;
	}    
	return point;
}		  
struct lst_w* make_list(short* err_f) {
	struct lst_w *point,*extra,*f;
	short* err_s;
	short err_str=0;
	err_s=&err_str;
	extra=read_block(err_s,err_f);
	if (*err_s==-1){
		return NULL;
	}	
	point=extra;
	f=point;
	while (err_str!=1) {
		extra=read_block(err_s,err_f);
		if (*err_s==-1){
			return NULL;
	    }	
		point->next=extra;
		point=extra;
	}   
	return f;
}			


void print_list(struct lst_w *point) {
	while (point!=NULL) {
		if (point->ptr!=NULL) {
			printf("[%s]\n",point->ptr);
		}
		else {
			printf("NULL\n");
		}		
		point=point->next;
	}
}

void delete_list(struct lst_w* point) {
	struct lst_w *buffer;
	while (point!=NULL) {
		buffer=point;
		point=point->next;
		free(buffer->ptr);
		free(buffer);
	}
}

int main() {
	struct lst_w *first;
	short error=0;
	short* err_f;
	err_f=&error;
	while (error!=1) {
		printf("-->");
	    first=make_list(err_f);
	    print_list(first);
	    delete_list(first);
    }
    return 0;
}

