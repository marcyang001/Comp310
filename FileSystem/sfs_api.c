
#define BLOCK_SIZE 512

#define MAX_FILES 100
#define MAX_FILE_LENGTH 16
#define MAX_FILE_EXTENTION 3
#define FILE_SYSTEM_SIZE 4


#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
//#include <slack/std.h>
//#include <slack/list.h>
#include "disk_emu.h"
#include "sfs_api.h"

static int NUM_BLOCKS = 3 + MAX_FILES;

// root directory
typedef struct { 

	char filename[MAX_FILE_LENGTH];
	char fileExtension[MAX_FILE_EXTENTION];
	int filesize; 
	int inode; // this is the inode number that will put into the inode table 
	int is_occupied; //the free BitMap;

}root_Dir;

//super block
typedef struct { 

	int magic; 
	int blockSize; 	//
	int fileSystemSize;  // # of blocks 
	int iNode_Table_length; // 12 direct pointers + 1 single indirect pointers 
	int rootDirectory; //Root Dictory i-Node # 

}super_block;

// inode table
typedef struct { 
	// pointer 1 - 12 are direct pointers
	int *pointer[12];  
	int *indirectPointer; 
	// pointer 13 is single indirect pointers 

}inode_table;

// file descriptor table
typedef struct {
	int open;
	int read;
	int write;

} fdt_list;


fdt_list fileDescriptor[MAX_FILES]; // each file is associated with a file descriptor

void set_fdt(int i, int open, int read, int write);

int mksfs(int fresh) { 

	if (fresh) {
		init_fresh_disk("SFS", BLOCK_SIZE, NUM_BLOCKS);
	}


	return 0;
}

int sfs_fopen(char *name) {

	return 0;
}

int sfs_fclose(int fileID){

	if (fileID > MAX_FILES || fileID < 0 || !fileDescriptor[fileID].open) {
		return -1;
	}

	set_fdt(fileID, 0, 0, 0);

	return 0;
}


int sfs_fwrite(int fileID, const char *buf, int length) {



	return 0;
}

int sfs_fread(int fileID, char *buf, int length) {



	return 0;
}

// seek changes read and write pointer by offset

int sfs_fseek(int fileID, int offset) {

	if (fileID > MAX_FILES || fileID < 0) {
		return -1;
	}
	set_fdt(fileID, 1, offset, offset);

	return 0;
}

int sfs_remove(char *file) {


	return 0;
}


int sfs_get_next_filename(char* filename) {


	return 0;
}

int sfs_GetFileSize(const char* path) {


	return 0;
}


void set_fdt(int i, int open, int read, int write) {
	fileDescriptor[i].open = open;
	fileDescriptor[i].read = read; 
	fileDescriptor[i].write = write;
}