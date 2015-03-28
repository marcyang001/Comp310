#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <math.h>
#define min(a,b) (a<b ? a : b)
#include "sfs_api.h"
#include "disk_emu.h"

int indirectpointers[PTR_INDEXBLOCK];
int FindindexBlock;

int firstTimeWrite(int fileID, const char *buf, int length, int baseBlock, int blockcounter);
int FindPreviousBlock(int fileID, int filesize, int baseBlock);
int updateInode(int fileID, int blockcounter, int writtenBlock);

int sfs_fwrite(int fileID, const char *buf, int length) {
		//the file size should be 0;
	//char tempBlock[BLOCK_SIZE];
	char firstBlock[BLOCK_SIZE];
	int nextFreeBlock;
	char *buffNew;
	int lastwritesize;

	int filesize =  fileNode[fileDescriptor[fileID].inode].size; 
	
	
	int base = fileNode[fileDescriptor[fileID].inode].pointer[0];
	//printf("BASE BLOCK %d\n", base);
	int bytesWritten;
	//counts how many new blocks need to associate with an inode pointer
	// the first inode pointer is 0, so we can increment this after we associate the inode pointer
	int blockcounter = 0; 

	if (fileID > MAX_FILES || fileID < 0 || !fileDescriptor[fileID].open) {
		return -1;
	}
	//adjust the rw_pointer to the end of the file 
	if(fileDescriptor[fileID].rw_pointer < fileNode[fileDescriptor[fileID].inode].size) {
		fileDescriptor[fileID].rw_pointer = fileNode[fileDescriptor[fileID].inode].size;	
	}
	




	int previousBlock = FindPreviousBlock(fileID, filesize, base);

	//printf("PREVIOUS BLOCK %d\n", previousBlock);


	int writePtr = fileDescriptor[fileID].rw_pointer; //rw_pointer position
	//printf("%d\n", length);
	//block where you start writing 
	int blockToStart = (writePtr - writePtr%BLOCK_SIZE)/BLOCK_SIZE;
	bytesWritten = min(BLOCK_SIZE - writePtr%BLOCK_SIZE, length); 
	
	//printf("NUM OF BYTE: %d\n", bytesWritten);
	//write the first block you start writing
	read_blocks(blockToStart+previousBlock, 1, &firstBlock);

	//fill in the rest of space in the block
	memcpy(firstBlock + writePtr%BLOCK_SIZE, buf, bytesWritten);
	//update the block
	write_blocks(blockToStart+previousBlock, 1, &firstBlock);
	//printf("WROTE SOMETHING IN THE FIRST BLOCK!!!!!!!!\n");
	
	//update inode and increment the blockcounter!!!! 
	updateInode(fileID, blockcounter, blockToStart+previousBlock);
	blockcounter++;

	//if there is not enough space to write in one block
	if(bytesWritten < length){
		
		//when you can fully write the block
		while((length - bytesWritten) >= BLOCK_SIZE) {

			buffNew = (char *)malloc(BLOCK_SIZE * sizeof(char));
			memcpy(buffNew, buf+ bytesWritten, 512);
			//printf("WROTE SOMETHING IN THE FIRST BLOCK2222222!!!!!!!!\n");
			//find the next free block 
			nextFreeBlock = FindFreeBlock();
			//printf("FREE BLOCK: %d\n", nextFreeBlock);
			//FindFreeBlock returns the actual block number
			if (nextFreeBlock < 0) {
				printf("No more space!!!!\n");
				//return -1;
				break;
			}
			
			
			write_blocks(nextFreeBlock, 1, &buffNew);

			free(buffNew);
			updateInode(fileID, blockcounter, nextFreeBlock);
			blockcounter++;
			bytesWritten = bytesWritten + 512;
			//remainingwritesize = length - bytesWritten;
			//printf("WROTE SOMETHING IN THE FIRST BLOCK33333!!!!!!!!\n");

		}
		//printf("NUM OF BYTES WRITTEN: %d\n", bytesWritten);
		
		// there are still some data left that are not enough to fill entire block
		if (bytesWritten <= length) {
			lastwritesize = length - bytesWritten;
			//printf("LAST PART TO WRITE!!! %d\n", lastwritesize);
			
			nextFreeBlock = FindFreeBlock();
			buffNew = (char *)malloc(lastwritesize * sizeof(char));
			memcpy(buffNew, buf+ bytesWritten, lastwritesize);
			write_blocks(nextFreeBlock, 1, &buffNew);
			free(buffNew);
			updateInode(fileID, blockcounter, nextFreeBlock);
			blockcounter++;

		}
		bytesWritten += lastwritesize;

	}		

	fileNode[fileDescriptor[fileID].inode].size = bytesWritten;

	write_blocks(2, 13, &fileNode);
	write_blocks(1, 1, &bitmap);
	write_blocks(FindindexBlock, 1, &indirectpointers);

	//bytesWritten += lastwritesize;
	
	return bytesWritten;
}



// return the actual last block number that stores the file!!!
int FindPreviousBlock(int fileID, int filesize, int baseBlock) {
	//determine which inode pointer we are looking for
	int m = filesize/BLOCK_SIZE;
	int lastBlock = -1;
	int indirect[PTR_INDEXBLOCK];
	int indexBlock;
	if (m <12) {
		lastBlock = fileNode[fileDescriptor[fileID].inode].pointer[m];
	}
	else if (m == 12 && fileNode[fileDescriptor[fileID].inode].indirect > 0) {
		//retrieve the index block
		indexBlock = fileNode[fileDescriptor[fileID].inode].indirect;
		read_blocks(indexBlock, 1, &indirect);
		lastBlock = indirect[0];
	}
	else if (m > 12 && fileNode[fileDescriptor[fileID].inode].indirect > 0){

		indexBlock = fileNode[fileDescriptor[fileID].inode].indirect;
		read_blocks(indexBlock, 1, &indirect);
		lastBlock = indirect[m-12];

	}

	//no need to update because all you do is read and you dont change anything

	return lastBlock;

}


int firstTimeWrite(int fileID, const char *buf, int length, int previousBlock, int blockcounter) {
	//buf[strlen(buf)-1] = '\0';


	//the file size should be 0;
	//char tempBlock[BLOCK_SIZE];
	char firstBlock[BLOCK_SIZE];
	int nextFreeBlock;
	char *buffNew;

	//int needed = ceil((double)length/BLOCK_SIZE);

	int writePtr = fileDescriptor[fileID].rw_pointer; //rw_pointer position
	//printf("%d\n", length);
	//block where you start writing 
	int blockToStart = (writePtr - writePtr%BLOCK_SIZE)/BLOCK_SIZE;
	int bytesWritten = min(BLOCK_SIZE - writePtr%BLOCK_SIZE, length); 
	
	//printf("NUM OF BYTE: %d\n", bytesWritten);
	//write the first block you start writing
	read_blocks(blockToStart+previousBlock, 1, &firstBlock);

	//fill in the rest of space in the block
	memcpy(firstBlock + writePtr%BLOCK_SIZE, buf, bytesWritten);
	//update the block
	write_blocks(blockToStart+previousBlock, 1, &firstBlock);
	//printf("WROTE SOMETHING IN THE FIRST BLOCK!!!!!!!!\n");
	
	//update inode and increment the blockcounter!!!! 
	updateInode(fileID, blockcounter, blockToStart+previousBlock);
	blockcounter++;

	//if there is not enough space to write in one block
	if(bytesWritten < length){
		
		//when you can fully write the block
		while((length - bytesWritten) >= BLOCK_SIZE) {

			buffNew = (char *)malloc(BLOCK_SIZE * sizeof(char));
			memcpy(buffNew, buf+ bytesWritten, 512);
			//printf("WROTE SOMETHING IN THE FIRST BLOCK2222222!!!!!!!!\n");
			//find the next free block 
			nextFreeBlock = FindFreeBlock();
			//printf("FREE BLOCK: %d\n", nextFreeBlock);
			//FindFreeBlock returns the actual block number
			if (nextFreeBlock < 0) {
				printf("No more space!!!!\n");
				//return -1;
				break;
			}
			
			
			write_blocks(nextFreeBlock, 1, &buffNew);

			free(buffNew);
			updateInode(fileID, blockcounter, nextFreeBlock);
			blockcounter++;
			bytesWritten = bytesWritten + 512;
			//remainingwritesize = length - bytesWritten;
			//printf("WROTE SOMETHING IN THE FIRST BLOCK33333!!!!!!!!\n");

		}
		// there are still some data left that are not enough to fill entire block
		if (bytesWritten <= length && (length - bytesWritten) <= BLOCK_SIZE) {
			nextFreeBlock = FindFreeBlock();
			buffNew = (char *)malloc((strlen(buf)%BLOCK_SIZE) * sizeof(char));
			memcpy(buffNew, buf+ bytesWritten, (strlen(buf)%BLOCK_SIZE));
			write_blocks(nextFreeBlock, 1, buffNew);
			free(buffNew);
			updateInode(fileID, blockcounter, nextFreeBlock);
			blockcounter++;
		}



	}		

	fileNode[fileDescriptor[fileID].inode].size = bytesWritten;

	write_blocks(2, 13, &fileNode);
	write_blocks(1, 1, &bitmap);
	write_blocks(FindindexBlock, 1, &indirectpointers);


	printf("NUM OF BYTES WRITTEN: %d\n", bytesWritten);
	return bytesWritten;
}


int updateInode(int fileID, int blockcounter, int writtenBlock) {


	//remember to update the inode table and the bitmap

	if (blockcounter < 12) {
		fileNode[fileDescriptor[fileID].inode].pointer[blockcounter] = writtenBlock;
	}
	else if (blockcounter == 12) { //create an index block
		FindindexBlock = FindFreeBlock(); //find a free block to store indices
		//indirect pointer stores the index block 
		fileNode[fileDescriptor[fileID].inode].indirect = FindindexBlock;
	
		//the first index of the array is to store the 13th block
		indirectpointers[0] = writtenBlock; 
		
	}
	else if (blockcounter > 12 && fileNode[fileDescriptor[fileID].inode].indirect != -1){
		//block counter > 13 
		// retrieve the index block 
		read_blocks(fileNode[fileDescriptor[fileID].inode].indirect, 1, &indirectpointers);
		indirectpointers[blockcounter - 12] = writtenBlock;

	}
	//update the index block 
	//update the inode table
	//update the bitmap

	


	return 0;

}
