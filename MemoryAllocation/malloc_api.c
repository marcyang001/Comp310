
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <memory.h>
#include "malloc_api.h"



char *my_malloc_error = "ERROR SETTING MALLOC!";


free_block head; //head of the free list
int freeblockcount = 0;
int policy;
void *base = NULL;

void addtoFreeList(t_block b);
void listDelete(t_block b);
void my_mallinfo();
void *valid_addr(void *p);
t_block merge (t_block first, t_block second);

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
					f = (free_block *)f->next;
				}
			}
			//if the last block is reached
			if (f->next == NULL) {
				if (!(f->length >= totalSize)) {
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
		printf("USING THE first fit policy\n");
		if (head != NULL) {
			while (f->next != NULL) {
				sign = 1;
				//check for the size is bigger or equal
				if (f->length == size) {
					break;
				}
				else {
					f = (free_block *)f->next;
				}
			}
			//if the last block is reached
			if (f->next == NULL) {
				if (!(f->length >= totalSize)) {
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

//if there is not free block that is big enough
t_block extend_heap(t_block last, size_t s) {
	t_block b;
	b = sbrk(0);
	if (sbrk(allocatedBlockMETA + s) == (void *) -1) {
			return (NULL);
	}
	b->length = s;
	b->prevFree = 1;
	b->nextFree = 1;
	return (b);

}


void *my_malloc(int size) {

	t_block b;
	t_block last;
	size_t s;
	s = size;
	if(base) { //check if there exists a base
		//first time finding a block
		//printf("TRYING TO FIND A BLOCK\n");
		b = find_free_block(s);

		if (b) {
		//	printf("A BLOCK IS FOUND!!!\n");
			b->length = s;
			b->prevFree = 1;
			b->nextFree = 1;


		}
		else {
		//	printf("CANNOT FIND A BLOCK\n");
			//printf("Using malloc for the second time, Cannot find a block Thus extend heap\n");
			last = sbrk(0);
			//No fitting block, extend the heap
			b = extend_heap(last, s);


		//	printf("ADDRESS EXTENDED: %p\n", b);
			if (!b)
				return my_malloc_error;
		}
	}
	else {
		//first time 
		b= sbrk(0);
		//printf("Initial address %p\n", b);
		sbrk(sizeof(struct block_meta)+ s);
		//printf("After address %p\n", sbrk(0));
		b->length = s;
		b->prevFree = 1;
		b->nextFree = 1;
		if(!b) {
			return my_malloc_error;
		}
		base = b;
	}
	

	return (b->data);
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
void my_mallopt(int pol){
	policy = pol;
}

void my_mallinfo() {

	//because of the head that is not free;
	printf("Number of free blocks: %d\n", freeblockcount);

}

void *valid_addr(void *p) {
	if (base != NULL) {
		if (p >= base && p <sbrk(0)) {
			return (t_block)p;
		}
	}
	return 0;
}
/*
t_block merge (t_block first, t_block second) {

	first->length += META_SIZE + second->length;
	return first;
}
*/
/*

void my_free(void *ptr) {
	

	printf("ENTER HERE\n");
	count++; //free block is incremented

	t_block tempFree1;
	t_block tempFree2;
	t_block current;
	t_block b;
	if (valid_addr(ptr)) {
		b = ptr; // b is the new free block
		b->prevFree = 0;
		addtoFreeList(b);
		printf("ENTER HERE 2\n");
		int flag =0;
		//merge the block if possible
		current = head;
		while (current != NULL) {

			//merge with the front block
			if (current + META_SIZE + current->length == b) {
				flag = 1;
				tempFree1 = merge(current, b);
				addtoFreeList(tempFree1);
			}
			//merge with the back block, check whether it has merge the front
			if (current- b->length - META_SIZE == b) {
				if (flag == 1) {
					tempFree2 = merge(tempFree1, current);
					listDelete(tempFree1);
				}
				else {
					tempFree2 = merge(b, current);

				}
				addtoFreeList(tempFree2);
				flag = 2;
			}
			current = current->next;
		}

		if (flag == 1 || flag ==2) {
			listDelete(b);
		}

	//free the end of the heap
		if ((b + b->length) == sbrk(0)) {
			listDelete(b);
			brk(b);
			count--; 
		}


	}


}
*/



