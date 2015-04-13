#define ALIGNMENT 8 // must be a power of 2
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))


#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <memory.h>

#include "malloc_api.h"
//int brk(void *end of data  segment) 

t_block head = NULL;

void addtoFreeList(t_block b) {
	t_block temp;
	temp = b;
	if (head == NULL) {
		head = temp;
		head->next = NULL;
		head->prev = NULL;
	}
	else {
		printf("Enter here!!!\n");
		t_block current = head;
		while (current != NULL) {
			
			if (current->next == NULL) {
				current->next = temp;
				current->next->prev = current;
				current->next->next = NULL;
			}
			current = current->next;
		}
	}


}


int main(int argc, char *argv[]) {

	t_block b;
	b= sbrk(0);
		//printf("Initial address %p\n", b);
	sbrk(sizeof(struct block_meta)+ 20 + 2);

	b->length = 20;
	addtoFreeList(b);
	
	t_block c;
	c= sbrk(0);
	//printf("Initial address %p\n", b);
	sbrk(sizeof(struct block_meta)+ 30 + 2);
	c->length = 30;
	printf("%d\n", head->length);

	addtoFreeList(c);

	printf("%d\n", head->next->length);

	t_block d;
	d = sbrk(0);
	sbrk(sizeof(struct block_meta)+ 40 + 2);
	d->length = 40;
	addtoFreeList(d);
	printf("%d\n", head->next->next->length);


	return 0;
}