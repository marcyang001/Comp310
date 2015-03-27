#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>

#include "sfs_api.h"
#include "disk_emu.h"


void remove_bit_inode(int inodeNumber);
void remove_bit_data(int blockNumber);
int sfs_remove(char *file) {
	int i, j; 
	int found = 0;
	//root_directory *temp = (root_directory *)malloc (sizeof(root_directory));
	//1. remove from the file descriptor table (if necessary)
	//2. remove from the inode bitmap
	//3. remove from the data block bitmap using the inode
	//4. change the name to NULL
	
	for (i = 0; i< MAX_FILES; i++) {
		//
		if (strcmp(files[i].filename, file) == 0){
			//memcpy(temp, files[i], sizeof(root_directory));
			//file found in the directory
			found = 1; 
			//check if the FD table 
			for (j = 0; j < MAX_FILES; j++){
				//if the inode if found in the FD table, 
				if (fileDescriptor[j].inode == files[i].inode){
					sfs_fclose(j); //close the FD table 
					break;
				}
			}
			// remove from the inode bitmap 
			remove_bit_inode(files[i].inode);
			//remove from the data block bitmap
			




			break;
		}
	}

	if (found) {
		return 1;
	}
	else 
		return 0;
}

void remove_bit_inode(int inodeNumber) {
	inodeNumber = inodeNumber +1; //the first inode starts with 0;
	const char constant = 0b1;
	int actualiNode = inodeNumber; 
	int shift = actualiNode % 8;
	int row = actualiNode/8;
	
	int compare = constant << (shift-1);

	bitmap.iNodeBitMap[row] = bitmap.iNodeBitMap[row] | compare;

}


void remove_bit_data(int blockNumber) {
	blockNumber = blockNumber +1; // the first inode start with 0
	const char constant = 0b1;
	int actualBlock = blockNumber; 
	int shift = actualBlock % 8;
	int row = actualBlock/8;
	
	int compare = constant << (shift-1);

	bitmap.dataBitMap[row] = bitmap.dataBitMap[row] | compare;


}