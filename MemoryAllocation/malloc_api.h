

#include <stdlib.h>


void *my_malloc(int size);
void my_free(void *ptr);
void my_mallopt(int policy);
void my_mallinfo();


char *my_malloc_error = "ERROR!";


struct block_meta { 
	char data[1];
	size_t length; 
	struct block_meta *next;
	struct block_meta *prev;
	int nextFree; //0 = not free, 1 = free 
	int prevFree;
	

};

// now the type is t_block, which is a pointer
//t_block is for the free blocks
typedef struct block_meta *t_block;



