#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>

#include "sfs_api.h"
#include "disk_emu.h"


void remove_bit_inode(int inodeNumber);
void remove_bit_data(int blockNumber);
int sfs_remove(char *file) {
	int i, j, k; // i = files j = fileDescriptor  k = num_pointer 
	int found = 0;
	int currentNode;
	int indexBlock;
	int num_pointer[PTR_INDEXBLOCK]; //store the actually block numbers, block # starts at 0
	//root_directory *temp = (root_directory *)malloc (sizeof(root_directory));
	//1. remove from the file descriptor table (if necessary)
	//2. remove from the data block bitmap using the inode
	//3. remove from the inode bitmap 
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
			//remove from the data block bitmap
			//check the first 12 direct pointers 
			currentNode = files[i].inode;
			for (j = 0; j < 12; j++){
				//using the or gate to turn off the bits so it doesnt matter
				remove_bit_data(fileNode[currentNode].pointer[j]);
			}
			//manipulate the indirect pointer to remove the rest of the data blocks
			//this is the index block
			indexBlock = fileNode[currentNode].indirect;
			if (indexBlock > 0) {
				//read the index block
				read_blocks(indexBlock, 1, &num_pointer);
				for (k = 0; k < PTR_INDEXBLOCK; k++) {
					//if the pointer actually points to a block
					if (num_pointer[k] > 0) {
						remove_bit_data(num_pointer[k]);
					}
				}

			}
			// remove from the inode bitmap 
			remove_bit_inode(files[i].inode);

			//change the file name to ""\0;
			memcpy(files[i].filename, "", sizeof(""));
			files[i].filename[19] = '\0';
			break;
		}
	}

	if (found) {
		return 0;
	}
	else 
		return -1;
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