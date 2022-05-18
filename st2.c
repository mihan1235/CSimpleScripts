#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
struct item 
{
	char *elem;
	struct item *next;
};
int len(struct item *list)
{
	int i=0;
	struct item *p;
	for(p=list;p->next!=NULL;p=p->next)
		i++;
	return i;
}
void enlarge(char **s1, int *n)
{
	int i,m;
	char *s2;
	m=2*(*n);
	s2=malloc(m*sizeof(char));
	for(i=0;i<*n;i++)
		s2[i]=(*s1)[i];
	*n=m;
	free(*s1);
	*s1=s2;
}
int form_wd(char **s, int *n)
{
	int i=0, t=0, c;
	for (c=getchar();c==' ';c=getchar());
	while ((c!=EOF)&&(c!='\n')&&(((c!='\t')&&(c!=' '))||t))
	{
		if (i<*n)
		{
			if (c=='"')
				if (t==0) t=1;
				else t=0;
			else 
			{
				(*s)[i]=c;
				i++;
			}
			c=getchar();
		}
		else 
		{
			enlarge(s,n);
		}
	}
	if (t!=0) 
	{
		(*s)=NULL;
		printf("warning: You haven't closed quotes\n");
	}
	*n=i;
	return (c);
}

char  *create_wd(int *f, int *l)
{ 
	int n=10,c;
	char *s;
	s=malloc(n*sizeof(char));
	char *wd;
	c=form_wd(&s,&n);
	if (s==NULL)
	{
		*l=1;
		return NULL;
	}
	if (c==EOF)
	{
		*f=1;
		if (n!=0) 
		{
			printf("\nerror: You've ended file inside the string\n");
			*f=2;
		}
		wd=NULL;
	}
	else 
	{
		if (c=='\n') 
			*l=1;
		if (n!=0)
		{
			wd=malloc((n+1)*sizeof(char));
			c=0;
			while (c<n)
			{
				wd[c]=s[c];
				c++;
			}
			wd[n]=0;
		}
		else
			wd=NULL;
	}
	free(s);
	return wd;		
}
struct item *add_item (struct item *l_elem, int *f, int *l)
{
	struct item *p;
	char *wd;
	wd=create_wd(f,l);
	if (wd!=NULL)
	{
		p=malloc(sizeof(*p));
		p->elem=wd;
		p->next=NULL;
		if (l_elem!=NULL)
			return(l_elem->next=p);
		else
			return(l_elem=p);
	}
	else
		return(l_elem);
}
struct item *create_list (int *f)
{
	struct item *p,*list;
	int l;
	l=0;
	list=NULL;
	while (list==NULL)
	{
		p=list=add_item(list,f,&l);
		if (f!=0) 
			break; 
	}
	while ((l==0)&&(*f==0))
		p=add_item(p,f,&l);
	if (*f==2)
		return (NULL);
	else 
		return(list);
}
char **create_arr (struct item *list)
{
	char **arr;
	int n,i;
	arr=NULL;
	arr=malloc((n=len(list)+1)*sizeof(list));
	for(i=0;i<n;i++)
	{
		arr[i]=list->elem;
		list=list->next;
	}
	arr[n]=NULL;
	return arr;
}	
void del_arr(char **arr, struct item *p)
{
	struct item *q;
	while (p!=NULL)
	{
		q=p;
		p=p->next;
		free(q->elem);
		free(q);
	}
	free(arr);
}
int cd(char **arr)
{
	char *c;
	if (arr[1]==NULL)
		c=getenv("HOME");
	else 
		if (arr[2]==NULL)
			c=arr[1];
		else 
			return -1;
	if (chdir(c)==-1)
		return -1;
	else
		return 0;
}
		
int main()	
{
	int f=0, status,p;
	char **arr, *com;
	struct item *list;
	while (f==0)
	{
		printf("<\n");
		list=create_list(&f);
		if (list!=NULL)
		{
			arr=create_arr(list);
			com=arr[0];
			if ((com[0]=='c')&&(com[1]=='d')&&(com[2]==0))
			{
				if (arr[2]!=NULL)
					printf("too much arguments");
				else
					if (cd(arr)!=0)
						perror(arr[1]);
					else;
			}
			else 	
			{
				p=fork();
				if (p==-1)
				{
					perror("fork");
					return 1;
				}
				if (p==0)
				{
					execvp(com,arr);
					perror(com);
					exit(1);
				}
				p=wait(&status);				
				del_arr(arr,list);
			}
		}
	}
	return (0);
}
