


#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
//#include <slack/std.h>
//#include <slack/list.h>
#include "disk_emu.h"
#include "sfs_api.h"






//free bitmap 0 for unoccupied, 1 for occupied 
//0b1100000





void initial_superBlock();
void initial_root();
void initial_Bitmap();
void initial_iNode();
void intialize_fdt();
int ffz (char valu);



int mksfs(int fresh) { 
	int i;

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

		initial_iNode();
		write_blocks(2, 13, &fileNode);
		intialize_fdt();
	}

	else { 
		init_disk("MyFileSystem", BLOCK_SIZE, NUM_BLOCKS);
		
		read_blocks(0, 1, &superBlock);	
		read_blocks(1, 1, &bitmap);	
		read_blocks(15, 1, &files[0]);

		read_blocks(16, 4, &files[1]); 
		read_blocks(2, 13, &fileNode);

	

		intialize_fdt();
		//read the first occupied data block of each file from the inode table 
		
		for (i = 1; i< MAX_FILES; i++) {
			if (fileNode[i].pointer[0] != -1 || fileNode[i].pointer[0] != 0) {
				read_blocks(fileNode[i].pointer[0], 1, &files[i+1]);
			}

				
		}
		//printf("ENTER HERE!!\n");
				

	}


	return 0;
}

int sfs_fclose(int fileID){

	if (fileID > MAX_FILES || fileID < 0 || !fileDescriptor[fileID].open) {
		return -1;
	}
	fileDescriptor[fileID].open = 0; 
	fileDescriptor[fileID].rw_pointer = 0;
	fileDescriptor[fileID].inode = -1; //reset the node to free 
	
	//fileNode[fileID].shiftForBit

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
	bitmap.dataBitMap[0] = 0b11100000;
	for (i = 1; i < 254; i++) {
		bitmap.dataBitMap[i] = 0b11111111;
	}
}

void initial_root() {

	strcpy(files[0].filename, ".");
	files[0].inode = 0;
	
	strcpy(files[1].filename, "..");
	files[1].inode = 0; 
	//fileNode[0].size = 0;
	fileNode[0].pointer[0] = 17;

}

void initial_iNode() {
	int i;
	for (i = 1; i < MAX_FILES; i++) {
		fileNode[i].size = -1;
		fileNode[i].pointer[0] = -1;
	}


}


void intialize_fdt() {
	int i;
	for (i = 0; i < MAX_FILES; i++) {
		fileDescriptor[i].open = 0;
		fileDescriptor[i].rw_pointer = 0;
		fileDescriptor[i].inode = -1;
	}

} 

int ffz (char valu) {

	int i;
	
	for (i = 0; i<=8; i++) {
		if (!(valu & (1 << i)))
			//return i;
			break;
	}
	if (i+1 <= 8) {
		return i+1;
	}
	else  
		return -1;
}