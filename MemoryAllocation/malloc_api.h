

#include <stdlib.h>


void *my_malloc(int size);
void my_free(void *ptr);
void my_mallopt(int policy);
void my_mallinfo();


char *my_malloc_error = "ERROR SETTING MALLOC!";


struct block_meta { 
	char data[1];
	short prevFree; //1 = used, 0 = free. the address is at block(starting address) + 2
	size_t length; 
	struct block_meta *next;
	struct block_meta *prev;
};

// now the type is t_block, which is a pointer
//t_block is for the free blocks
typedef struct block_meta *t_block;



