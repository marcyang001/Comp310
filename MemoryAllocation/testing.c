#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <memory.h>
#include "malloc_api.h"
//#include "malloc_api.c"

int main(int argc, char *argv[]) {
	
	
	//my_mallopt(0);

	printf("%p\n", sbrk(0));
	printf("size of META data: %d\n", allocatedBlockMETA);
	char *p = (char *)my_malloc(10);
	//printf("tag address %p and value is %u\n", (p+2), *(p+2));
	printf("first call %p\n", p);
	//printf("Base address %p\n", base);
	char *q = (char *)my_malloc(10);
	printf("SECOND CALL MALLOC %p!! \n", q);
	//printf("tag address %p and value is %u\n", (q-2), *(q-2));
	//printf("tag address %p and value is %u\n", (q+2), *(q+2));
	char *r = (char *)my_malloc(10);
	printf("THIRD TIME CALLING MALLOC %p!!!!\n", r);
	//printf("tag address %p and value is %u\n", (r+2), *(r+2));
	//printf("tag address %p and value is %u\n", (r+26), *(r+26));
	//printf("tag address %p and value is %u\n", (sbrk(0)-2), *((short*)(sbrk(0)-2)));

	int i; 
	for (i = 0; i <10; i++) {
		p[i] = 'a';
	}
	p[10] = '\0';
	//strcpy(q, p);
	//strcpy(r, q);
	printf("%s string length %d\n", p, strlen(p));
	//my_free(q);

	my_mallinfo();


	//printf("%p\n", valid_addr(p));


	return 0;
}