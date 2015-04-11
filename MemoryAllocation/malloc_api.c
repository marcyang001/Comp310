#define META_SIZE sizeof(struct block_meta)
#define align4(x) (((((x)-1) >> 2) <<2) +4)
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
//t_block last = NULL;
//int brk(void *end of data  segment) --> set the program break;
//int sbrk(intptr_t increment) ---> increment the program break or increment heap space

t_block find_free_block(t_block *last, size_t size) {
	t_block f = base;
	while (f && !(f->next && f->length >= size)) {
		*last = f;
		f = f->next;
	}
	return (f);
}

//if there is not free block that is big enough, 
t_block extend_heap(t_block last, size_t s) {
	t_block temp = last;
	//the new extended block is no longer free
	t_block b;
	b = sbrk(0);
	if (sbrk(META_SIZE + s) == (void *) -1) {
		//sbrk fails, go to die 
		return (NULL);
	}
	b->length = s;
	//next block is free
	b->next = NULL;
	if (temp != NULL) {
		temp->next = b;
		temp->nextFree = 0;
	}
	//the next tag from the previous block is not free 
	//last->nextFree = 0;

	return (b);
}

void split_block(t_block b, size_t s) {
	t_block newBlock;
	newBlock = b + s; //address of the new block
	newBlock->length = b->length - s - META_SIZE;
	newBlock->next = b->next;
	newBlock->prevFree = 0;
	b->length =s;
}

void *my_malloc(int size) {

	t_block temp;
	t_block b;
	
	t_block last;
	size_t s;
	s = size;
	if(base) {
		printf("TRYING TO Find a block!!!!!!!!!!!!!!!!!!!!!\n");
		//first time finding a block
		last = base;
		b = find_free_block(&last, s);

		if (b) {
			// maybe we can split
			if ((b->length - s) >= (META_SIZE + 4)) {
				split_block(b, s);
				b->nextFree = 1;
			}	
		}
		else {
			printf("Using malloc for the second time, Cannot find a block\n");
			//No fitting block, extend the heap
			b = extend_heap(last, s);
			if (!b)
				return (NULL);
		}
	}
	else {

		printf("Here 3\n");
		//first time 

		b= sbrk(0);
		printf("Initial address %p\n", b);
		sbrk(sizeof(struct block_meta)+ s);
		printf("After address %p\n", sbrk(0));
		b->length = s;

		if(last) {
			last = b;
			last->next = NULL;
		}


		if(!b) {
			printf("HERE NULL\n");
			return (NULL);
		}
		base = b;
	}
	return (b->data);
}


int main(int argc, char *argv[]) {
	
	
	printf("%p\n", sbrk(0));
	
	char *p = (char *)my_malloc(10);
	printf("first call %p\n", p);
	char *q = (char *)my_malloc(10);
	printf("SECOND CALL MALLOC %p!! \n", q);
	int i; 
	for (i = 0; i <10; i++) {
		p[i] = 'a';
	}
	p[10] = '\0';

	strcpy(q, p);
	
	printf("%s string length %d\n", p, strlen(p));
	printf("%p\n", p);
	printf("%s string length %d\n", q, strlen(q));
	printf("%p\n", q);


	return 0;
}