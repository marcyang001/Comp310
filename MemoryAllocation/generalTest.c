#define ALIGNMENT 8 // must be a power of 2
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))


#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <memory.h>

#include "malloc_api.h"
//int brk(void *end of data  segment) 
int freeblockcount;
int policy =1;
int currentFreeSpace;
free_block head = NULL;
void *find_free_block(int size);
void addtoFreeList(void *address) {
	
	int first = 0;
	free_block temp = head;
	free_block thisBlock = address;
	free_block current;

	if (temp == NULL) {
		head = thisBlock;
		head->next = NULL;
		head->prev = NULL;
		head->length = thisBlock->length;

	}
	else {
		while (temp->next != NULL) {
			temp =(free_block) temp->next;
		}
		if (temp->next == NULL) {
			temp->next = (int*)thisBlock;
			thisBlock->prev = (int*)temp;
			temp =(free_block) temp->next;
			temp->length = thisBlock->length;
			temp->next =NULL;
		}

	}
	currentFreeSpace += thisBlock->length;
	freeblockcount++;
}
/*
void listDelete(t_block b) {
	t_block temp = b;
	if (b->next == NULL) {
		printf("Delete the last one \n");
		b->next = NULL;
		b->prev->next = NULL;
		b->prev = NULL;

	}
	if (temp->next != NULL) {
		b->prev->next = temp->next;
		b->next->prev = temp->prev;
	}
	b->next = NULL;
	b->prev = NULL;

}
*/
int main(int argc, char *argv[]) {

	free_block b;
	b= sbrk(0);
	sbrk(sizeof(struct block_meta)+ 20);
	printf("address B %p\n", b);
	b->length = 20;
	

	addtoFreeList(b);
	printf("HEAD length :%d\n", head->length);

	free_block c;
	c= sbrk(0);
	sbrk(sizeof(struct block_meta)+ 30);
	printf("address C %p\n", c);
	c->length = 30;
	addtoFreeList(c);
	free_block t = (free_block)(head->next);
	//t = (free_block)(t->prev);
	printf("%d\n", t->length);

	t_block d;
	d = sbrk(0);
	sbrk(sizeof(struct block_meta)+ 40);
	printf("address D %p\n", d);
	d->length = 40;
	addtoFreeList(d);
	t = (free_block)(t->next);
	//t = (free_block)(t->prev);
	printf("%d\n", t->length);

	t_block e;
	e = sbrk(0);
	sbrk(sizeof(struct block_meta)+ 50);
	printf("address E %p\n", e);
	e->length = 50;
	addtoFreeList(e);
	t = (free_block)(t->next);
	printf("%d\n", t->length);
	//t = (free_block)(t->prev);


	printf("NUMBER OF FREE BLOCKS: %d\n", freeblockcount);
	printf("CURRENT FREE SPACE %d\n", currentFreeSpace);
	find_free_block(19);



	return 0;
}


void *find_free_block(int size) {
	printf("Enter here policy %d\n", policy);
	int totalSize = size + sizeof(allocatedBlockMETA);
	free_block f = head;
	int sign = 0;
	//first fit 
	if (policy == 0) {
		printf("USING THE first fit policy\n");
		if (head != NULL) {
			while (f->next != NULL) {
				sign = 1;
				//check for the size is bigger or equal
				if (f->length >= size) {
					break;
				}
				else {
					f = (free_block)f->next;
				}
			}
			//if the last block is reached
			if (f->next == NULL) {
				if (!(f->length >= size)) {
					f = NULL;
				}
				if(!sign)
					head = NULL;
			}
		}
		else {
			f = NULL;
		}
	}
	//best fit
	if (policy == 1) {
		printf("USING THE best fit policy\n");
		if (head != NULL) {
			while (f->next != NULL) {
				sign = 1;
				//check for the size is bigger or equal
				if (f->length == size) {
					break;
				}
				else {
					f = (free_block)f->next;
				}
			}
			//if the last block is reached
			if (f->next == NULL) {
				if (!(f->length == size)) {
					f = NULL;
				}
				if(!sign)
					head = NULL;
			}
		}
		else {
			f = NULL;
		}
	}

	printf("address of free block %p\n", f);
	return (f);
}
