
#define BLOCK_SIZE 512
#define MAX_FILES 100
#define MAX_FILE_LENGTH 16
#define MAX_FILE_EXTENTION 4
#define NUM_BLOCKS 2048
#define INODE_BITMAP 4
#define DATA_BITMAP 64

#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
//#include <slack/std.h>
//#include <slack/list.h>
#include "disk_emu.h"
#include "sfs_api.h"





typedef struct { 

	char filename[MAX_FILE_LENGTH + 4]; //4 for .XXX extension
	//char fileExtension[MAX_FILE_EXTENTION];
	int inode; // this is the inode number that will put into the inode table 


}root_directory;



//super block
typedef struct { 

	int magic; 
	int blockSize; 	//
	int fileSystemSize;  // # of blocks 
	int iNode_Table_length; // 12 direct pointers + 1 single indirect pointers 
	int rootDirectory; //Root Dictory i-Node # 
	int unused[123];

}super_block;



typedef struct { 
	char mode; 
	char link_cnt; 
	short  uid; 
	int gid; 
	int size; //file size 
	int pointer[12];  
	int indirect; 

}i_node; 


// file descriptor table
typedef struct {
	int open;
	int rw_pointer;
	int inode;

} fdt_list;


typedef struct { 
	//we need 13 because we need 100 inodes, each char = 8bits. so ceil(100/8) = 13
	char iNodeBitMap[13]; 
	//int iNodeBitMap[INODE_BITMAP]; // we need 128 bytes for 100 files 
	//int dataBitMap[DATA_BITMAP]; //(2040 - 16)/32
 	char dataBitMap[254]; // 2048 - 16/ 8 
// data_block, each block size is 512
}s_bitmap;

s_bitmap bitmap;

super_block superBlock;

//100 inodes correspond to 100 files, 4 extra are to accumulate 13 byte bitmap
i_node fileNode[104];


root_directory files[107];

//file descriptor table
fdt_list fileDescriptor[MAX_FILES]; // each file is associated with a file descriptor

//free bitmap 0 for unoccupied, 1 for occupied 
//0b1100000





void initial_superBlock();
void initial_root();
void initial_Bitmap();

int mksfs(int fresh) { 

	if (fresh) {
		init_fresh_disk("MyFileSystem", BLOCK_SIZE, NUM_BLOCKS);
		initial_superBlock();

		//Write the directory structure to the 1st block in the FS.
		write_blocks(0, 1, &superBlock);
		
		//initialize the inode table
		initial_Bitmap();
		write_blocks(1, 1, &bitmap);


		//initialize inode for the root directory 
		initial_root();
		write_blocks(15, 1, &files[0]);
		write_blocks(16, 1, &files[1]);

		write_blocks(2, 13, &fileNode);

	}

	else { 
		init_disk("MyFileSystem", BLOCK_SIZE, NUM_BLOCKS);
		
		read_blocks(0, 1, &superBlock);	
		read_blocks(1, 1, &bitmap);	
		read_blocks(15, 5, &files[0]); 
		read_blocks(2, 13, &fileNode);


	}


	return 0;
}

int sfs_fopen(char *name) {
	int i,j, iNodeIndex;
	int currentInode;
	int iNodeNumber;
	for (i = 0; i<MAX_FILES; i++){
		if (strcmp(name, files[i].filename) ==0){
			//search for empty descriptor table 
			currentInode=files[i].inode;
			for (j = 0; j < MAX_FILES; j++) {
				if (fileDescriptor[j].open == 0) {
					fileDescriptor[j].open = 1;
					fileDescriptor[j].rw_pointer = fileNode[currentInode].size;
					fileDescriptor[j].inode=currentInode;
					break;
				}
			}
			break;
		}
	}
	//create a new file
	if (i==MAX_FILES){
		//find free inode
		char temp = 0b11111111;
		int sign;

		for (iNodeIndex = 0; iNodeIndex < INODE_BITMAP; iNodeIndex++) {
			//printf("Entered here!!!! %d\n", ffs(bitmap.iNodeBitMap[iNodeIndex]));
			if(bitmap.iNodeBitMap[iNodeIndex] != 0b00000000) {
				printf("Free INODE EXISTS\n");
				sign = 1;
				int FREEiNode = ffs(bitmap.iNodeBitMap[iNodeIndex]);
				printf("Free inode: %d, inode index %d\n", FREEiNode, iNodeIndex);

				temp = temp << FREEiNode;
				//turn off the inode bit 
				bitmap.iNodeBitMap[iNodeIndex] = temp & bitmap.iNodeBitMap[iNodeIndex];
				printf("Free inode: %d, inode index %d\n", ffs(bitmap.iNodeBitMap[iNodeIndex]), iNodeIndex);
				iNodeNumber = FREEiNode * (iNodeIndex + 1) -1;
				
				printf("Inode number: %d\n", iNodeNumber);
				
				//add file to root directory with the free inode
				files[iNodeNumber].inode = iNodeNumber;
				strcpy(files[iNodeNumber].filename, name);
				//update the new file size 
				fileNode[iNodeNumber].size = 0;

				break;
			}	
		}
		if (sign != 1) {
			printf("No free inode\n");
		}


		
		
		
		
		//find free block, add block number to inode.
		//find free file descriptor
		//update bitmap
	}



	return 0;
}

int sfs_fclose(int fileID){

	if (fileID > MAX_FILES || fileID < 0 || !fileDescriptor[fileID].open) {
		return -1;
	}
	fileDescriptor[fileID].open = 0; 
	fileDescriptor[fileID].rw_pointer = 0;


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

	fileDescriptor[fileID].open = 1;
	fileDescriptor[fileID].rw_pointer = offset;

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

void initial_superBlock() {
	
	//initialize the super block 

	superBlock.magic = 0xAABB0005; 
	superBlock.blockSize = 512; 
	superBlock.fileSystemSize = MAX_FILES;
	superBlock.iNode_Table_length = 11;
	superBlock.rootDirectory = 0; //first inode is always 0 -- dont change

}

void initial_Bitmap() {
	
	printf("Initialize bitmap\n");

	int i; 
	//the first bit of the bitmap is taken by the root directory

	// 1 for not taken, 0 for occupied
	bitmap.iNodeBitMap[0] = 0b11111110;	

	for (i = 1; i < 13; i++) {
		bitmap.iNodeBitMap[i] = 0b11111111;
	}

	//assign 5 blocks to the root directory 
	bitmap.dataBitMap[0] = 0b11111100;
	for (i = 1; i < 254; i++) {
		bitmap.dataBitMap[i] = 0b11111111;
	}
}

void initial_root() {

	strcpy(files[0].filename, ".");
	files[0].inode = 0;
	files[1].inode = 0; 
	strcpy(files[1].filename, "..");

	//fileNode[0].size = 0;
	fileNode[0].pointer[0] = 17;

}
