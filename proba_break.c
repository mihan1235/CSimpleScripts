#include <stdio.h>
struct lst_w {
	char* ptr;
	struct lst_w *next;
};

void print_list( struct lst_w *point) {
		while (point!=NULL) {
			if (point->ptr!=NULL) {
				printf("%s\n",point->ptr);
			}
			else {
				printf("NULL\n");
			}		
			point=point->next;
		}
}

int main()
{
	struct lst_w* f;
	f=malloc(sizeof(*f));
	f->ptr=NULL;
	f->next=NULL;
    print_list(f);		
	return 0;
}

