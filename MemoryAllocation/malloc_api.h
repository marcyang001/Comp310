
#include <stdlib.h>


struct block_meta1 { 
	char data[1];
	short prevFree; //1 = used, 0 = free. the address is at block(starting address) + 2
	int length; 
	int *next;
	int *prev;
};

typedef struct block_meta1 *free_block; 

struct block_meta {
	char data[1];
	short prevFree;
	int length;
	short nextFree;
};


// now the type is t_block, which is a pointer
typedef struct block_meta *t_block;




void *my_malloc(int size);
void my_free(void *ptr);
void my_mallopt(int policy);
void my_mallinfo();
void *valid_addr(void *p);
t_block merge (t_block first, t_block second);




#define allocatedBlockMETA sizeof(struct block_meta)

#define freeBlockMETA sizeof(struct block_meta1)

