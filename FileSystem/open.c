#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>

#include "sfs_api.h"
#include "disk_emu.h"

int FindFreeNode();
int FindFreeBlock();


int sfs_fopen(char *name) {
	int i,j;
	int currentInode;
	//int FREEDATABLOCK;
	//int actualBlock;
	int sign = 0;
	for (i = 0; i<MAX_FILES; i++){
		//printf("GO IN HERE %d!!!\n", i);
		printf("Name: %s filename %s\n", name, files[i].filename);
		if (strcmp(name, files[i].filename) ==0){
			sign = 1;
			printf("File is found in the disk\n");
			//search for empty descriptor table 
			currentInode=files[i].inode;
			printf("File node #: %d and index %d\n", currentInode, i);
			for (j = 0; j < MAX_FILES; j++) {
				if (fileDescriptor[j].open == 0 && fileDescriptor[j].inode == 0) {
					fileDescriptor[j].open = 1;
					fileDescriptor[j].rw_pointer = fileNode[currentInode].size;
					fileDescriptor[j].inode = currentInode;
					//printf("Node # of the file: %d\n", fileDescriptor[j].inode);
					
					break;
				}
			}
			break;
		}
	}

	
	//create a new file
	if (sign == 0){
		printf("Create a new file\n");
		//find free inode
		int x = FindFreeNode();
		

		printf("Free node #: %d\n", x);
		int y = FindFreeBlock();
		//printf("Free Block %d\n", y);

		if ( x != -1 && y != -1) {



				//update data block 
				strcpy(files[x+1].filename, name);
				files[x+1].filename[19] = '\0';
				files[x+1].inode = x;
				write_blocks(y, 1, &files[x+1]);


				//printf("DONE WRITING FILE TO THE BLOCK!!!\n");


				//update inode of the file
				fileNode[x].size = 0;
				fileNode[x].pointer[0] = y;
				write_blocks(2, 13, &fileNode);

				write_blocks(1, 1, &bitmap);
				
				

				//update file descriptor table

				for (j = 0; j < MAX_FILES; j++) {
					if (fileDescriptor[j].open == 0) {
						fileDescriptor[j].open = 1;
						fileDescriptor[j].rw_pointer = 0;
						fileDescriptor[j].inode = x;
							//printf("Node # of the file: %d\n", fileDescriptor[j].inode);
						break;
					}
				}		


	/*
		// find the free data block in the data bitmap
		for (dataIndex = 0; dataIndex < DATA_BITMAP; dataIndex++) {
			if (bitmap.dataBitMap[dataIndex] != 0b00000000) {
				printf("Free DATA BLOCK EXISTS\n");
				signData = 1;
				FREEDATABLOCK = ffs(bitmap.dataBitMap[dataIndex]);
				
				printf("Free data block: %d, block index %d\n", FREEDATABLOCK, dataIndex);

				char temp = constant << FREEDATABLOCK;

				bitmap.dataBitMap[dataIndex] = temp & bitmap.dataBitMap[dataIndex];

				printf("Free block: %d, data index %d\n", ffs(bitmap.dataBitMap[dataIndex]), dataIndex);



				break;
			}
		}
		if (signData != 1) {
			printf("No free data block\n");
		}

		if (signData == 1) {
			printf("Block operation!!! \n");
			//add file to root directory with the free inode
			actualBlock = FREEDATABLOCK + 15; // 15th block is the root
			//the first pointer of the inode of that file has to pointer to the right block
			fileNode[iNodeNumber +1].pointer[0] = actualBlock;
			printf("actual block: %d\n", actualBlock);

		}

		
		
		
		
		//find free block, add block number to inode.
		//find free file descriptor
		
		//update the inode table
		write_blocks(2, 13, &fileNode);
		write_blocks(actualBlock, 1, &files);

		//update the bitmap
		write_blocks(1, 1, &bitmap);
	*/
		}	
	}
	
	if (sign == 1) {
		return 1;
	}
	else {
		return 0;
	}


	
}



int FindFreeNode() {
	int iNodeNumber, iNodeIndex;
	int FREEiNode;
	int available = 0;
	char temp;
	//find free inode
	char constant = 0b11111111;
	//check for free inode 
	for (iNodeIndex = 0; iNodeIndex < INODE_BITMAP; iNodeIndex++) {
		//printf("Entered here!!!! %d\n", ffs(bitmap.iNodeBitMap[iNodeIndex]));
		if(bitmap.iNodeBitMap[iNodeIndex] != 0b00000000) {
			//printf("Free INODE EXISTS\n");
			available = 1;
			FREEiNode = ffs(bitmap.iNodeBitMap[iNodeIndex]);
			temp = constant << FREEiNode;
			bitmap.iNodeBitMap[iNodeIndex] = temp & bitmap.iNodeBitMap[iNodeIndex];
				
			//printf("Free inode bit: %d, inode index %d\n", ffs(bitmap.iNodeBitMap[iNodeIndex]), iNodeIndex);
			
			iNodeNumber = FREEiNode + (iNodeIndex * 8) -1;
			//the first inode is 0

			//printf("Inode number: %d\n", iNodeNumber);
			break;
		}
	}


	if (available == 1) {
		return iNodeNumber;
	}
	else {
		return -1;
	}

}

int FindFreeBlock() { 

	int dataIndex,FREEDATABLOCK, blockNumber, actualBlock; 
	char constant = 0b11111111;
	char temp;
	int available = 0;
	// find the free data block in the data bitmap
	for (dataIndex = 0; dataIndex < DATA_BITMAP; dataIndex++) {
		if (bitmap.dataBitMap[dataIndex] != 0b00000000) {
			available = 1;
			//printf("Free DATA BLOCK EXISTS\n");
			FREEDATABLOCK = ffs(bitmap.dataBitMap[dataIndex]);
				
			//printf("Free data block: %d, block index %d\n", FREEDATABLOCK, dataIndex);

			temp = constant << FREEDATABLOCK;

			bitmap.dataBitMap[dataIndex] = temp & bitmap.dataBitMap[dataIndex];

			blockNumber = FREEDATABLOCK + (dataIndex * 8) - 1;
			//first block starts with 0;

			//printf("Free block: %d, data index %d\n", ffs(bitmap.dataBitMap[dataIndex]), dataIndex);

			break;
		}
	}
	if (available == 1) {
		actualBlock = blockNumber + 15;
		return actualBlock;
	}
	else {
		return -1;
	}

	

}