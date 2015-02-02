
#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#define MAX_LINE 80

char history[10][80]; //store the 10 previous commands 
int numComm[10]; //store the command number 
int comStatus[10];

int counter; // index of the array
int number;   // command number 

void addCommands(int counter, int number, char *input);
void printCommand();

 /*setup() reads in the next command line, separating it into distinct tokens using whitespace as delimiters. 
   setup() sets the args parameter as a null-terminated string 

*/


void setup(char inputBuffer[], char *args[], int *background, int *rlc) 
{
	int length, // # of characters in the command line 
	i, 	// loop inde for accessing inputBuffer array 
	start, // index where beginning of the next command parameter is 
	ct;  //index of where to place the next parameter into args[] 

	ct = 0; 

// read what the user enters on the command line 

length = read(STDIN_FILENO, inputBuffer, MAX_LINE);

char *tempBuffer;
strcpy(tempBuffer, inputBuffer);
if (tempBuffer[0] == 'r' && tempBuffer[2] != '\0') {
	//*rlc = 1; 
	int i; 
	char *str;
		// scan the array in reverse the to 
		//look for the most recent command starting with 'x'
	for (i = 9; i >=0; i--) {
		if (tempBuffer[2] == history[i][0]) {
			strcpy(inputBuffer, history[i]);
			printf("%d ", numComm[i]); 
			printf("%s\n", history[i]);	
			break; //get out of the loop 
		}

	}
	inputBuffer[strlen(history[i])] = '\0';
	length = strlen(history[i]);
	//strcpy(hist)

}




start = -1; 
if (length == 0) 
	exit(0);

if (length < 0) {
	perror ("error reading the command"); 
	exit(-1);
}

// examine every character in the input Buffer 

for ( i = 0; i < length; i++) {
	switch (inputBuffer[i]) {
		case ' ':
		case '\t' :		// argument separators 
		   if (start != -1) {
			args[ct] = &inputBuffer[start]; //set up pointer 
			ct++; 
		   }
		   inputBuffer[i] = '\0'; //add a null char; make a C string
		   start = -1; 
		   break;

		case '\n':
			if (start != -1) {
				args[ct] = &inputBuffer[start];
				ct++;
			}
			inputBuffer[i] = '\0';
			args[ct] = NULL; //no more argument to this command
			break;

		default:
			if (start == -1 && inputBuffer[i]!='&')
				start = i;
			if (inputBuffer[i] == '&') {
				*background = 1;
				//inputBuffer[i] = '&';
				inputBuffer[i] = '\0';
			}



		}
	}
	args[ct] = NULL; //just in case the input line was > 80 
	
	//printf("%s\n", args[1]);
	// when r l\n  is entered
/*
if (strcmp (args[0],"r") == 0 && args[1] != NULL) {
	//*rlc = 1; 
	int i; 
	char *str;
		// scan the array in reverse the to 
		//look for the most recent command starting with 'x'
	for (i = 9; i >=0; i--) {
		if (args[1][0] == history[i][0]) {
			strcpy(str, history[i]);
			printf("%d ", numComm[i]); 
			printf("%s\n", history[i]);	
			break; //get out of the loop 
		}

	}
	printf("%s\n", str);
	//char sep[2] = " ";
	//char *token;
	//int argNumber = 0; 
	//token = strtok(str, sep);
	//printf("%s\n", token);
	strcpy(args, str);
	//printf("argument = %s\n", args[0]);
	//token = strtok(NULL, sep);
	//token = strtok(str, sep);
	//printf("%s\n", token);
	//strcpy(args[1], token);
	//args[2] = NULL;

}
*/
}



int main (void) 
{
	int quantity = 30;
	char inputBuffer[MAX_LINE]; // buffer to hold the command entered 
	int background; // equals 1 if a command is followed by '&'

	char *args[MAX_LINE/+1]; //command line (of 80) has max of 40 arguments 
	int rlc = 0; // notify is r 'x' command is entered 

	counter = 0;  //initialize the counter 
	number = 1; // first command is numbered 1 
	

	while (1) {   //program terminates normally inside setup
		background = 0; 
		printf (" COMMAND -> \n");

		
		setup(inputBuffer, args, &background, &rlc); // get next command 

		printf("%s\n", args[0]);

		/*String MANIUPULATION  */
		char tempString[80];
		strcpy(tempString, inputBuffer);
		int j = 1; 
		int n = strlen(inputBuffer);
		while (args[j] != NULL) {
			strcat(tempString, " ");
			n = n + strlen(" ");
			strcat(tempString, args[j]);
			n = n + strlen (args[j]);
			j++;
		}
		tempString[n] = '\0';
		//printf("%s\n", tempString);
		/*String MANIUPULATION ENDS */

	
		addCommands(counter, number, tempString);
		counter++;
		number++;

		int child_status = fork(); 
		int status;
		int execStatus;

		if (child_status == 0) {
			
			if (strcmp ("pwd", inputBuffer) !=0 && strcmp("history", inputBuffer) != 0 && strcmp("cd", inputBuffer) != 0) {
				execStatus = execvp(args[0], args);
				printf ("%d\n", execStatus);
				//printf("function in child %d\n", strcmp ("pwd", inputBuffer));
			}
			//printf("%s\n", inputBuffer);


			
		}
		else { 	// parent process 
			//waitpid(child_status);

			if (strcmp(args[0], "exit")==0){
					kill(0, SIGKILL);
					exit(1);
			}

			if (strcmp(args[0], "history") == 0) {
				printCommand();
				//printf("%s\n", history[2]);
			}
			//if (strcmp(args[0], "cd") == 0) {
			//	char directory[200];
			//	int ret;
			//	printf("enter a directory\n");
			//	fgets(directory, 199, stdin);	
			//	if (directory != NULL) {
			//		ret = chdir(directory);
			//	}
			//	else {
			//		continue;
			//	}	
					
			//}


			if (strcmp(args[0], "pwd") == 0) {
				char *cwd = getcwd(args[0], MAX_LINE);

				if (cwd != NULL) {
					printf ("%s\n", cwd);
				}
				else {
					printf("Issues with pwd : %s\n", strerror(errno));
				}
					
					
			}


			if (background == 0) {
				
				pid_t w = waitpid(child_status, &status, 0);
				if ( w == -1) {
					perror("waitpid error"); 
					exit(EXIT_FAILURE);
				}
				//printf("%d\n", i);
			}
			
			
		}

	}

}

void addCommands(int counter, int number, char *input) {

//	input[length-1] = '\0';
	if (counter < 10) {
				numComm[counter] = number;
				comStatus[counter] = 1; //assume the command is good  
				strcpy(history[counter], input);
				//history[counter][length-1] = '\0';
			}
			else {
				int i; 
				for (i = 0; i < 9; i++) {
					*(numComm + i) = *(numComm+i+1);
					*(comStatus + i) = *(comStatus + i + 1);
					strcpy(history[i], history[i+1]);
					//history[i][strlen(history[i])-1] = '\0';
				}
				numComm[9] = number;
				comStatus[9] = 1; 
				strcpy(history[9], input);
				//history[counter][length-1] = '\0';
			}

}

void printCommand() {

	int i;
	for (i = 0; i< 10; i++) {
		printf("%d ", numComm[i]);
		printf("%s\n", history[i]);
	}

	printf("\n");

}




