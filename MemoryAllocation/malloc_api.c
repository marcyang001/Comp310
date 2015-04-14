
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <memory.h>
#include "malloc_api.h"



char *my_malloc_error = "ERROR SETTING MALLOC!";


free_block head = NULL; //head of the free list
int freeblockcount;
int policy;
int currentFreeSpace;
void *base = NULL;

void addtoFreeList(void *address);
void listDelete(void *addr);
void my_mallinfo();
int valid_addr(void *p);
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
	else if (temp == head && head->next == NULL) {
		printf("Delete the only block in the list\n");
		head->next = NULL;
		head->prev = NULL;
		head = NULL;
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



void my_mallopt(int pol){
	policy = pol;
}

void my_mallinfo() {
	printf("\n**********My Malloc Info*******************\n");
	//because of the head that is not free;
	printf("Number of free blocks: %d\n", freeblockcount);
	printf("CURRENT FREE SPACE %d\n", currentFreeSpace);
	int endaddress = (int)sbrk(0) - (int)base;
	printf("NUMBER OF BYTES ALLOCATED %d\n", endaddress - freeblockcount *freeBlockMETA - currentFreeSpace);
	printf("********************************************\n");
}


/*
void *valid_addr(void *p) {
	if (base != NULL) {
		if (p >= base && p <sbrk(0)) {
			return (t_block)p;
		}
	}
	return 0;
}





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



