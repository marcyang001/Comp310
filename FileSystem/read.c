#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <math.h>

#include "sfs_api.h"
#include "disk_emu.h"

int sfs_fread(int fileID, char *buf, int length) {
	int i;
	char tempBlock[BLOCK_SIZE];
	//char *tempBlock = (char *)malloc(BLOCK_SIZE * sizeof(char));
	int IndexBlock[128]; // store the index block if necessary
	int numBlocks = ceil(length/BLOCK_SIZE);
	
	//char *tempBuf = (char *)malloc(BLOCK_SIZE *numBlocks * sizeof(char));
	
	char tempBuf[BLOCK_SIZE *numBlocks]; //create a temp buffer to store 
	//int numBytesRead = length;
	int offset = 0;
	/*
	1. retrieve all the data blocks associated with that file inode
	2. store in the the tempbuf 
	3. store all the data (length) into char *buf 
	
	*/
	if (fileDescriptor[fileID].open == 0 && fileDescriptor[fileID].inode < 0 && fileID < 0 && fileID > (MAX_FILES)) {
		return -1;
	}
	
	//check if its open before anything 
	else if (fileDescriptor[fileID].open > 0) {
		 // cannot read more than what is on the disk. so modified length to what it can
		int modifiedlength = 0;
		if(fileNode[fileDescriptor[fileID].inode].size < length){
			modifiedlength = fileNode[fileDescriptor[fileID].inode].size;
			//printf("USED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		} else {
			modifiedlength = length;
		}


		//printf("READ!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		//this is the number of blocks required to retrieve
		
		//scan the pointers in the inode
		
		int iNode = fileDescriptor[fileID].inode;
		int blockNumber;
		int data_ind; //stores the index block

		if (numBlocks <= 12 ) {
			for (i = 0; i < numBlocks; i++) {


					blockNumber = fileNode[iNode].pointer[i];

					read_blocks(blockNumber, 1, &tempBlock);
					//printf("ENTEREREREERE HERER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 11111\n");
					//printf("%s!!!!!!!!!!!!!!!!!!!!!!\n\n", tempBlock);
					//copy the content of each block 
					sprintf(tempBuf + offset, "%s", tempBlock);
					//memcpy(tempBuf + offset, tempBlock, strlen(tempBlock));
					//printf("ENTEREREREERE HERER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 11111\n");
					//free(tempBlock);

					offset += BLOCK_SIZE;
			}
			tempBuf[BLOCK_SIZE *numBlocks-1] = '\0';

			if (tempBuf != NULL && numBlocks <= 12) {
				memcpy(buf, tempBuf, length);
				//buf[length] = '\0';
				//printf("BYTE READ!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
				return length;
				//return 0;
				
			}
			else 
				return -1;

		}

		else {
			printf("TOOOOOOO LARGE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			return -1;
		}

		/*	
			//for indirect block
			else {
				//retrieve the index block
				data_ind = fileNode[iNode].indirect;
				read_blocks(data_ind, 1, &IndexBlock);
				j = 0;
				while(IndexBlock[j] > 0) {
					read_blocks(IndexBlock[j], 1, &tempBlock);
					sprintf(tempBuf + offset, "%s", tempBlock);
					printf("ENTEREREREERE HERER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 2222\n");
					offset += BLOCK_SIZE;

					j++;
				}
			
			}*/
		

		//printf("ENTEREREREERE HERER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 2222\n");
		/*now we have everything in the tempBuf, 
			we can store the # of bytes we need to char* buf 
		*/
		//sprintf(buff, "%s", tempBuf-(sizeof(tempBuf)-length));
	
		
		//buf[length-1] = '\0';

	}
	else {

		return -1;
	}

	// copy the data to buf 


	
}
