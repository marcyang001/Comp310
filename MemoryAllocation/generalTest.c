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
int policy = 0;
int currentFreeSpace;
free_block head = NULL;
void testcases();
void *find_free_block(int size);
void addtoFreeList(void *address) {
	
	free_block temp = head;
	free_block thisBlock = address;

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

void listDelete(void *addr) {
	
	free_block temp = addr;
	free_block iterator1;
	free_block iterator2;
	//delete the head
	if(temp == head) {
		printf("delete the first block\n");
		head = (free_block)head->next;
		head->prev = NULL;

	}
	//delete the last block in the free list 
	else if (temp->next == NULL) {
		printf("Delete the last one \n");
		iterator1 = (free_block)(temp->prev);
		iterator1->next = NULL;
		temp->prev = NULL;
		//temp->prev->next = NULL;
		//temp->prev = NULL;

	}
	//delete a block that is in the middle of the list
	else {
		printf("Delete the one in the middle\n");
		//deallocate the pointer from the previous block
		iterator1 = (free_block)(temp->prev);
		iterator1->next = temp->next;
		//deallocated the pointer from the next block
		iterator2 = (free_block)(temp->next);
		iterator2->prev = temp->prev;
		//deallocate the current block
		temp->prev = NULL;
		temp->next = NULL;


	}
	currentFreeSpace -= temp->length;
	freeblockcount--;
}


int main(int argc, char *argv[]) {

	free_block b;
	b= sbrk(0);
	sbrk(sizeof(struct block_meta1)+ 20);
	printf("address B %p\n", b);
	b->length = 20;
	addtoFreeList(b);
	printf("HEAD length :%d and head address %p\n", head->length, head);

	free_block c;
	c= sbrk(0);
	sbrk(sizeof(struct block_meta1)+ 30);
	printf("address C %p\n", c);
	c->length = 30;
	addtoFreeList(c);
	free_block t = (free_block)(head->next);
	printf("%d\n", t->length);

	t_block d;
	d = sbrk(0);
	sbrk(sizeof(struct block_meta1)+ 40);
	printf("address D %p\n", d);
	d->length = 40;
	addtoFreeList(d);

	t= (free_block)(t->next);
	printf("%d\n", t->length);
	//listDelete(c);
	listDelete(d);
	if (head->next == NULL){
		printf("block removed!!!!\n");
	}
	else {
		t = (free_block)(head->next);
		printf("HEAD NEXT %d and address %p\n", t->length, t);
	}




	///////////////////////////////////////////////////////////

	printf("head Length %d\n", head->length);
	printf("NUMBER OF FREE BLOCKS: %d\n", freeblockcount);
	int endaddress = (int)sbrk(0) - (int)head;
	printf("NUMBER OF BYTES ALLOCATED %d\n", endaddress - freeblockcount *freeBlockMETA - currentFreeSpace);
	printf("CURRENT FREE SPACE %d\n", currentFreeSpace);
	//testcases();
	printf("%p\n", sbrk(0));
	return 0;
}

void testcases() {
		free_block b;
	b= sbrk(0);
	sbrk(sizeof(struct block_meta1)+ 20);
	printf("address B %p\n", b);
	b->length = 20;
	

	addtoFreeList(b);
	printf("HEAD length :%d\n", head->length);

	free_block c;
	c= sbrk(0);
	sbrk(sizeof(struct block_meta1)+ 30);
	printf("address C %p\n", c);
	c->length = 30;
	addtoFreeList(c);
	free_block t = (free_block)(head->next);
	//t = (free_block)(t->prev);
	printf("%d\n", t->length);

	t_block d;
	d = sbrk(0);
	sbrk(sizeof(struct block_meta1)+ 40);
	printf("address D %p\n", d);
	d->length = 40;
	addtoFreeList(d);
	t = (free_block)(t->next);
	//t = (free_block)(t->prev);
	printf("%d\n", t->length);

	t_block e;
	e = sbrk(0);
	sbrk(sizeof(struct block_meta1)+ 50);
	printf("address E %p\n", e);
	e->length = 50;
	addtoFreeList(e);
	t = (free_block)(t->next);
	t = (free_block)(t->prev);
	printf("%d\n", t->length);
	//t = (free_block)(t->prev);

	//void listDelete(void *addr)
	printf("NUMBER OF FREE BLOCKS: %d\n", freeblockcount);
	printf("CURRENT FREE SPACE %d\n", currentFreeSpace);
	find_free_block(19);
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
