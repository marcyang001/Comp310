#define META_SIZE sizeof(struct block_meta)

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <memory.h>
#include "malloc_api.h"
#include <memory.h>


void *base = NULL;
//int brk(void *end of data  segment) --> set the program break;
//int sbrk(intptr_t increment) ---> increment the program break or increment heap space






void *my_malloc(int size) {


	void *p = sbrk(0); 

	void *request = sbrk(size);

	if (request == (void *)-1) {
		return NULL; 
	}
	else {
		
		assert(p == request);
		return p;
	}

}


int main(int argc, char *argv[]) {
	
	/*
	printf("%p\n", sbrk(0));
	char *p = (char *)my_malloc(10);
	char *q = (char *)malloc(100);
	int i; 
	for (i = 0; i <10; i++) {
		p[i] = 'a';
	}
	strcpy(q, p);
	printf("%s\n", q);
	*/

	t_block b;
	t_block c;
	b = sbrk(0);
	printf("%p\n", b);
	sbrk(sizeof(struct block_meta)+ 20);
	b->length = 20;
	
	printf("%p\n", sbrk(0));

	printf("%d\n", b->length);
	b->next = c;
	sbrk(sizeof(struct block_meta)+ 20);
	c->length = 30;
	printf("%p\n", sbrk(0));

	printf("%d\n", b->next->length);



	return 0;
}

t_block find_free_block(t_block *last, size_t size) {
	t_block b = base;
	while (b && !(b->next && b->length >= size)) {
		*last = b;
		b = b->next;
	}
	return (b);
}

//if there is not free block that is big enough, 
t_block extend_heap(t_block last, size_t s) {
	//the new extended block is no longer free
	s_block b;
	b = sbrk(0);
	if (sbrk(META_SIZE + s) == (void *) -1) {
		//sbrk fails, go to die 
		return (NULL);
	}
	b->length = s;
	//next block is free
	b->nextFree = 0;

	//the next tag from the previous block is not free 
	last->nextFree = 0;
	
}

