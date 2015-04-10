#define align4(x) (((((x)-1) >> 2) <<2) +4)

#include <stdlib.h>


void *my_malloc(int size);
void my_free(void *ptr);
void my_mallopt(int policy);
void my_mallinfo();


char *my_malloc_error = "ERROR!";


struct block_meta { 

	size_t length; 
	struct block_meta *next;
	struct block_meta *prev;
	int nextFree; //0 = not free, 1 = free 
	int prevFree;

};

// now the type is t_block, which is a pointer
//t_block is for the free blocks
typedef struct block_meta *t_block;


// this is for the allocated data blocks
struct block_meta2 {

	size_t length;
	int nextFree; //0 = not free, 1 = free 
	int prevFree;
};

typedef struct block_meta2 *s_block;

