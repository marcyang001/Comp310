#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>

#include "sfs_api.h"
#include "disk_emu.h"

int FindFreeNode();
int FindFreeBlock();


int sfs_fopen(char *name) {
	//name[strlen(name)-1] = '\0';

	int i,j; // j is the new fd number and k is the used fd number 
	int k;
	int currentInode;
	int availableFD = 0;
	
	//int FREEDATABLOCK;
	//int actualBlock;
	int sign = 0;
	int open = 0;
	for (i = 0; i<MAX_FILES; i++){
		//printf("GO IN HERE %d!!!\n", i);
		//printf("Name: %s filename %s\n", name, files[i].filename);
		if (strcmp(name, files[i].filename) ==0){
			sign = 1;
			//printf("File is found in the disk\n");


			//check if the file is already opened
			for (k = 0; k < MAX_FILES; k++) {
				if (files[i].inode == fileDescriptor[k].inode) {
					open = 1;
					break;
				}
			}


			if (open == 0) {
				//search for empty descriptor table 
				currentInode=files[i].inode;
				//printf("File node #: %d and index %d\n", currentInode, i);
				for (j = 0; j < MAX_FILES; j++) {
					if (fileDescriptor[j].occupied == 0) {
						availableFD = 1;
						fileDescriptor[j].open = 1;
						fileDescriptor[j].rw_pointer = fileNode[currentInode].size;
						fileDescriptor[j].inode = currentInode;
						fileDescriptor[j].occupied = 1;
						//printf("FD #: %d\n", j);
						//printf("Node # of the file: %d\n", fileDescriptor[j].inode);
						
						break;
					}
				}
				//break;
			}
			break;
		}
	}

	
	//create a new file
	if (sign == 0){
		//printf("Create a new file\n");
		//find free inode
		int x = FindFreeNode();
		

		//printf("Free node #: %d\n", x);
		int y = FindFreeBlock();
		//printf("Free Block %d\n", y);

		if ( x != -1 && y != -1) {



				//update data block 
				strcpy(files[x+1].filename, name);
				files[x+1].filename[20] = '\0';
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
					if (fileDescriptor[j].occupied == 0) {
						availableFD = 1;
						fileDescriptor[j].open = 1;
						fileDescriptor[j].rw_pointer = 0;
						fileDescriptor[j].inode = x;
						fileDescriptor[j].occupied = 1;
							//printf("Node # of the file: %d\n", fileDescriptor[j].inode);
						break;
					}
				}		


		}	
	}
	
	if (sign == 1 && open == 1) {
		//printf("Success\n");
		//printf("already opened\n");
		return k;
	}
	else if (sign == 1 && open == 0 && availableFD == 1) {
		//printf("No new descriptor\n");
		return j;
	}
	else if (sign == 0 && availableFD == 1) {
		//printf("new file\n");
		return j;
	}
	else 
		return -1;


	
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
