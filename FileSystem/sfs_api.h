
#define BLOCK_SIZE 512
#define MAX_FILES 100
#define MAX_FILE_LENGTH 16
#define MAX_INODE 104	//100 inodes correspond to 100 files, 4 extra are to accumulate 13 byte bitmap
#define NUM_BLOCKS 4096
#define INODE_BITMAP 13
#define DATA_BITMAP 510
#define NUM_DIRECT_PTR 12


int mksfs(int fresh);
int sfs_fopen(char *name);
int sfs_fclose(int fileID);
int sfs_fwrite(int fileID, const char *buf, int length);
int sfs_fread(int fileID, char *buf, int length);
int sfs_fseek(int fileID, int offset);
int sfs_remove(char *file);
int sfs_get_next_filename(char* fname);
int sfs_GetFileSize(const char* path);


typedef struct { 

	char filename[MAX_FILE_LENGTH + 5]; //4 for .XXX extension
	//char fileExtension[MAX_FILE_EXTENTION];
	int inode; // this is the inode number that will put into the inode table 


}root_directory;

root_directory files[107]; 

//super block
typedef struct { 

	int magic; 
	int blockSize; 	//
	int fileSystemSize;  // # of blocks 
	int iNode_Table_length; // 12 direct pointers + 1 single indirect pointers 
	int rootDirectory; //Root Dictory i-Node # 
	int unused[123];

}super_block;
super_block superBlock;


typedef struct { 
	char mode; 
	char link_cnt; 
	short  uid; 
	int shiftForBit; 
	int size; //file size 
	int pointer[NUM_DIRECT_PTR];  // first pointer of the inode points to the first data block of the file
	int indirect; 

}i_node; 

i_node fileNode[MAX_INODE];

// file descriptor table
typedef struct {
	int open;
	int rw_pointer;
	int inode;
	int occupied;

} fdt_list;


typedef struct { 
	//we need 13 because we need 100 inodes, each char = 8bits. so ceil(100/8) = 13
	char iNodeBitMap[INODE_BITMAP]; 
	//int iNodeBitMap[INODE_BITMAP]; // we need 128 bytes for 100 files 
	
 	char dataBitMap[DATA_BITMAP]; // 2048 - 17/ 8 
// data_block, each block size is 512
}s_bitmap;

s_bitmap bitmap;



//file descriptor table
fdt_list fileDescriptor[MAX_FILES+7]; // each file is associated with a file descriptor