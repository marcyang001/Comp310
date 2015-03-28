


#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "disk_emu.h"
#include "sfs_api.h"


int currentFile; //keeps track of getNextFile method 



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
		currentFile = 2;
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
		//because the first block of each file store the information
		for (i = 1; i< MAX_FILES; i++) {
			if (fileNode[i].pointer[0] > 0) {
				read_blocks(fileNode[i].pointer[0], 1, &files[i+1]);
			}

				
		}
		currentFile = 2;

	}


	return 0;
}

int sfs_fclose(int fileID){

	int i;
	if (fileID > MAX_FILES || fileID < 0) {
		printf("Error here???\n");
		return -1;
	}
	//closes an already closed file
	else if(fileDescriptor[fileID].occupied != 1)
	{	
		printf("Error in closed\n");
    	return -1;
  	}
  	else {
  		for (i = 0; i< MAX_FILES; i++) {
			
  			if (files[i].inode == fileDescriptor[fileID].inode) {
				fileDescriptor[i].open = 0; 
				fileDescriptor[i].rw_pointer = 0;
				fileDescriptor[i].inode = -1; //reset the node to free
				fileDescriptor[i].occupied = 0;
				break;
			} 
		}

		return 0;
	}
}



// seek changes read and write pointer by offset

int sfs_fseek(int fileID, int offset) {

	//printf("FILEID : %d\n", fileID);

	if (fileID > MAX_FILES || fileID < 0) {
		return -1;
	}

	if (fileDescriptor[fileID].open == 1){
		fileDescriptor[fileID].rw_pointer = offset;
		return 1;
	}
	else {
		fprintf(stderr, "Error: File not opened or file associated with a file Descriptor");
		return -1;
	}
	

	
}




int sfs_get_next_filename(char* fname) {
	fname[19] = '\0';
	int compare = (files[currentFile+1].filename[0] <= 'z' && files[currentFile+1].filename[0] >= 'a')
		|| (files[currentFile+1].filename[0] <= 'Z' && files[currentFile+1].filename[0] >= 'A');
	//current file start at 2
	if (compare) {
		printf("Enter here\n");
		strcpy (fname, files[currentFile+1].filename);
		printf("Next file is %s\n", fname);
		if (fileNode[files[currentFile+1].inode].size == 0) {
			currentFile++;
			return currentFile;
		}
	}
	else if (!compare) {
		printf("Finish looping\n");
		return 0;
	
	}
	
		//printf("No file in the next directory\n");
		
	return 0;
		
}

int sfs_GetFileSize(const char* path) {
	int i;
	int fileSize;
	for (i = 0; i< MAX_FILES; i++) {
		//if the file in found in the directory
		if (strcmp(path, files[i].filename)==0) {
			fileSize = fileNode[files[i].inode].size;
			return fileSize; 
			break;
		}
	}



	return -1;
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
	
	//printf("Initialize bitmap\n");

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
	int i, j;
	for (i = 1; i < MAX_FILES; i++) {
		fileNode[i].size = -1;
		//initialize the first 12 direct pointers to -1
		for (j = 0; j < NUM_DIRECT_PTR; j++) {
			fileNode[i].pointer[j] = -1;
		}
		//initialize the indirect pointer to -1
		fileNode[i].indirect = -1;
		
	}
}


void intialize_fdt() {
	int i;
	for (i = 0; i < MAX_FILES; i++) {
		fileDescriptor[i].open = 0;
		fileDescriptor[i].rw_pointer = 0;
		fileDescriptor[i].inode = -1;
		fileDescriptor[i].occupied = 0;
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