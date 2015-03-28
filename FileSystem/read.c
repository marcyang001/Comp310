#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <math.h>

#include "sfs_api.h"
#include "disk_emu.h"

int sfs_fread(int fileID, char *buf, int length) {
	int i;
	int tempBlock[512];
	//int IndexBlock[128]; // store the index block if necessary
	int numBlocks = ceil(length/BLOCK_SIZE);
	char tempBuf[BLOCK_SIZE *numBlocks]; //create a temp buffer to store 
	int numBytesRead = 0;
	int offset = 0;
	/*
	1. retrieve all the data blocks associated with that file inode
	2. store all the data into char *buf 
	
	*/

	
	//check if its open before anything 
	if (fileDescriptor[fileID].open != 0) {
		//this is the number of blocks required to retrieve
		
		//scan the pointers in the inode
		
		int iNode = fileDescriptor[fileID].inode;
		int blockNumber;
		if (numBlocks <= 12) {
			for (i = 0; i < numBlocks; i++) {
				blockNumber = fileNode[iNode].pointer[i];
				read_blocks(blockNumber, 1, &tempBlock);
				memcpy(tempBuf + offset, tempBlock, strlen(tempBlock));
				offset += BLOCK_SIZE;
			}
		}



	}
	else {



	}



	


	return 0;
}
