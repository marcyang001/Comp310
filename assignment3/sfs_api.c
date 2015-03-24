
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


// root directory
typedef struct { 

	char filename[MAX_FILE_LENGTH];
	char fileExtension[MAX_FILE_EXTENTION];
	int filesize; 
	int inodeNumber; // this is the inode number that will put into the inode table 
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


int mksfs(int fresh) { 





	return 0;
}

int sfs_fopen(char *name) {

	return 0;
}

int sfs_fclose(int fileID){

	return 0;
}


int sfs_fwrite(int fileID, const char *buf, int length) {



	return 0;
}

int sfs_fread(int fileID, char *buf, int length) {

	return 0;
}


int sfs_fseek(int fileID, int offset) {


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