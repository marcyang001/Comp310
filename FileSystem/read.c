#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <math.h>

#include "sfs_api.h"
#include "disk_emu.h"

int sfs_fread(int fileID, char *buf, int length) {
	int i, j;
	char tempBlock[BLOCK_SIZE];
	//char *tempBlock = (char *)malloc(BLOCK_SIZE * sizeof(char));
	int IndexBlock[128]; // store the index block if necessary
	int numBlocks = ceil(length/BLOCK_SIZE);
	
	//char *tempBuf = (char *)malloc(BLOCK_SIZE *numBlocks * sizeof(char));
	
	char tempBuf[BLOCK_SIZE *numBlocks]; //create a temp buffer to store 
	int numBytesRead = length;
	int offset = 0;
	/*
	1. retrieve all the data blocks associated with that file inode
	2. store in the the tempbuf 
	3. store all the data (length) into char *buf 
	
	*/

	
	//check if its open before anything 
	if (fileDescriptor[fileID].open != 0 && fileDescriptor[fileID].inode >0) {
		//this is the number of blocks required to retrieve
		
		//scan the pointers in the inode
		
		int iNode = fileDescriptor[fileID].inode;
		int blockNumber;
		int data_ind; //stores the index block
		for (i = 0; i < 12; i++) {


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
		}

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



	


	return numBytesRead;
}
