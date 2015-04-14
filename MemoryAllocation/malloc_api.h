
#include <stdlib.h>


struct block_meta1 { 
	char data[1];
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
/////my helper methods 
void *find_free_block(int size);
void *valid_addr(void *p);




#define allocatedBlockMETA sizeof(struct block_meta)	// 12 bytes

#define freeBlockMETA sizeof(struct block_meta1) // 16 bytes 

