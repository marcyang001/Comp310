
#define MAX_FNAME_LENGTH 20   /* Assume at most 20 characters (16.3) */

/* The maximum number of files to attempt to open or create.  NOTE: we
 * do not _require_ that you support this many files. This is just to
 * test the behavior of your code.
 */
#define MAX_FD 100 

/* The maximum number of bytes we'll try to write to a file. If you
 * support much shorter or larger files for some reason, feel free to
 * reduce this value.
 */
#define MAX_BYTES 30000 /* Maximum file size I'll try to create */
#define MIN_BYTES 10000         /* Minimum file size */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "sfs_api.h"
#include "disk_emu.h"
//#include "sfs_test.c"
char *rand_name() 
{
  char fname[MAX_FNAME_LENGTH];
  int i;

  for (i = 0; i < MAX_FNAME_LENGTH; i++) {
    if (i != 8) {
      fname[i] = 'A' + (rand() % 26);
    }
    else {
      fname[i] = '.';
    }
  }
  fname[i] = '\0';
  return (strdup(fname));
}

int main(int argc, char **argv) {

  int tmp;
  char *names[2];
  int fds[2];
  int error_count = 0;
  //int filesize[MAX_FD];
  mksfs(1);

  //mksfs(1);
  //sfs_fopen("hey.txt");
  


mksfs(0);

int i;
for (i = 0; i < 2; i++) {
    names[i] = rand_name();
    names[0][19] = '\0';
    fds[i] = sfs_fopen(names[i]);
    printf("Index: %d\n", i);
    printf("FD1 : %d\n", fds[i]);
    if (fds[i] < 0) {
      fprintf(stderr, "ERROR: creating first test file %s\n", names[i]);
      error_count++;
    }
    tmp = sfs_fopen(names[i]);
    printf("Index: %d\n", i);
    printf("FD1 : %d\n", tmp);
    if (tmp >= 0 && tmp != fds[i]) {
      fprintf(stderr, "ERROR: file %s was opened twice\n", names[i]);
      error_count++;
    }
  }
  int s = sfs_fclose(tmp);
  printf("%d\n", s);

  int b = sfs_GetFileSize(names[0]);

  printf("Size of the file : %d\n", b);

  //char a[20]; 
  //int b = sfs_get_next_filename(a);
  //printf("get nextfile: %s\n", a);
  //printf("Status, %d\n", b);

  //b = sfs_get_next_filename(a);
  //printf("Status, %d\n", b);

  //printf("get nextfile: %s\n", a);
  //b = sfs_get_next_filename(a);
  //printf("Status, %d\n", b);
  //printf("get nextfile: %s\n", a);
/*
  names[0] = rand_name();
  printf("%s\n", names[0]);
  int fd = sfs_fopen(names[0]);
  printf("FD1 : %d\n", fd);
  if (fd < 0) {
    fprintf(stderr, "ERROR: creating first test file %s\n", names[0]);
    error_count++;
  }
  tmp = sfs_fopen(names[0]);
  printf("%s\n", names[0]);
    //printf("Index: %d\n", i);
    printf("FD1 : %d\n", tmp);
    if (tmp >= 0 && tmp != fd) {
      fprintf(stderr, "ERROR: file %s was opened twice\n", names[0]);

    }
*/
/*
  names[0] = rand_name();
  names[0][19] = '\0';
  int fd = sfs_fopen(names[0]);
  printf("%s\n", names[0]);

  if (fd < 0) {
    fprintf(stderr, "ERROR: creating first test file %s\n", "hey.txt");
    error_count++;
  }


  tmp = sfs_fopen(names[0]);

  printf("%s\n", names[0]);
  if (tmp >= 0 && tmp != fd) {
      fprintf(stderr, "ERROR: file %s was opened twice\n", "hey.txt");

  }
  printf("fd: %d\n", fd);
  printf("fd: %d\n", tmp);

*/

  return 0;
}