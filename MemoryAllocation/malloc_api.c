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
t_block head = NULL; //head of the free list
//t_block last = NULL;
//int brk(void *end of data  segment) --> set the program break;
//int sbrk(intptr_t increment) ---> increment the program break or increment heap space
void addtoFreeList(t_block b);


t_block find_free_block(t_block *last, size_t size) {

	t_block f = head;
	while (f != NULL &&(f->length >= size)) {
		*last = f;
		f = f->next;
	}
	printf("PASSING IN FIND A FREE BLOCK %p\n", f);
	return (f);
}

//if there is not free block that is big enough
t_block extend_heap(t_block last, size_t s) {
	printf("ENTER HERE to Extend HEAP!!!!\n");
	t_block b;
	b = sbrk(0);
	//if the previous block is free
	if (*(short *)(sbrk(0) - 2) == 0) {
		if (sbrk(META_SIZE + (s - last->length) + 2) == (void *) -1) {
			//sbrk fails, go to die 
			return (NULL);
		}
		b->length = s;
		b->prevFree = 1;
	}
	else {
		if (sbrk(META_SIZE + s + 2) == (void *) -1) {
			return (NULL);
		}
		b->length = s;
		b->prevFree = 1;

	}
	void *endtag = sbrk(0) - 2;
	(*(short *)endtag) = 1;

	printf("EXtended address %p\n", b);
	return (b);

}
// block b is the free block, s is the size that fits into this free block
void split_block(t_block newBlock, t_block b, size_t s) {
	printf("SPLITTING THE BLOCK!!!\n");
	int currentblocklength = b->length;
	newBlock = b + META_SIZE + s + 2; //address of the new block
	newBlock->length = b->length - (s + 2) - (META_SIZE);
	b->length =s;
	b->prevFree = 1;
	//hard code the end tag
	void *endtagB = newBlock - 2;
	(*(short *)endtagB) = 1;

	//new free block!!!
	newBlock->prevFree = 0;
	void *endtagNewblock = b + META_SIZE + currentblocklength;
	//hard code the end tag
	(*(short *)endtagNewblock) = 0;
	addtoFreeList(newBlock);
	
}

void *my_malloc(int size) {

	t_block tempFree;
	t_block b;

	t_block last;
	size_t s;
	s = size;
	if(base) { //check if there exists a base
		//first time finding a block
		printf("TRYING TO FIND A BLOCK\n");
		last = base; //set the last block to the base address 
		b = find_free_block(&last, s);

		if (b) {
			printf("A BLOCK IS FOUND!!!\n");
			// maybe we can split--> must reserve 20 bytes for meta_data and 4 additional space for length
			if ((b->length - s) >= (META_SIZE + 6)) {

				split_block(tempFree, b, s);
				
			}	
		}
		else {
			printf("CANNOT FIND A BLOCK\n");
			//printf("Using malloc for the second time, Cannot find a block Thus extend heap\n");
			last = sbrk(0);
			//No fitting block, extend the heap
			b = extend_heap(last, s);
//			void *endtag = sbrk(0) - 2;
//			(*(short *)endtag) = 1;

			printf("ADDRESS EXTENDED: %p\n", b);
			if (!b)
				return my_malloc_error;
		}
	}
	else {
		//first time 
		b= sbrk(0);
		//printf("Initial address %p\n", b);
		sbrk(sizeof(struct block_meta)+ s + 2);
		//printf("After address %p\n", sbrk(0));
		b->length = s;
		b->prevFree = 1;
		//hard code the the second boundary tag. make sure its at address next block - 2
		void *address = sbrk(0) - 2;
		(*(short *)address) = 1;
		//printf("the address is %p and the value at that address is %u\n", address, (*(short *)address));
		//the first block is the head of the free list regardless of whether its occupied
		//b->next = NULL; //start of the free list 
		//b->prev = NULL;

		if(!b) {
			return my_malloc_error;
		}
		//set the base --> the base will always be the first block
		addtoFreeList(b);

		printf("PROGRAM ENDS\n");
		base = b;
	}
	

	return (b->data);
}


int main(int argc, char *argv[]) {
	
	
	printf("%p\n", sbrk(0));
	//printf("size of META data: %d\n", META_SIZE);
	char *p = (char *)my_malloc(10);
	//printf("first call %p\n", p);
	printf("Base address %p\n", base);
	char *q = (char *)my_malloc(10);
	//printf("SECOND CALL MALLOC %p!! \n", q);
	printf("tag address %p and value is %u\n", (q-2), *(q-2));
	printf("THIRD TIME CALLING MALLOC!!!!\n");
	char *r = (char *)my_malloc(10);
	printf("tag address %p and value is %u\n", (r-2), *(r-2));
	//printf("tag address %p and value is %u\n", (r+26), *(r+26));
	printf("tag address %p and value is %u\n", (sbrk(0)-2), *((short*)(sbrk(0)-2)));

	int i; 
	for (i = 0; i <10; i++) {
		p[i] = 'a';
	}
	p[10] = '\0';
	//strcpy(q, p);
	//strcpy(r, q);
	printf("%d\n", META_SIZE);
	printf("%s string length %d\n", p, strlen(p));
	//printf("%p\n", p);
	//printf("%s string length %d\n", q, strlen(q));
	//printf("%p\n", q);
	//printf("%s string length %d\n", r, strlen(r));
	//printf("tag address %p and value is %u\n", (r-2), *(r-2));
	//printf("%p\n", r);


	return 0;
}

void addtoFreeList(t_block b) {
	//printf("ENTER Free list \n");
	t_block temp;
	temp = b;
	if (head == NULL) {
	//	printf("ENTER HEAD!!!!\n");
		head = temp;
		head->next = NULL;
		head->prev = NULL;
	}
	else {
	//	printf("Enter here!!!\n");
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
