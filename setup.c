
#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#define MAX_LINE 80

char history[10][80]; //store the 10 previous commands 
int numComm[10]; //store the command number 
int comStatus[10]; //store 1 if the command is good else -1

int counter; // index of the array
int number;   // command number 

void addCommands(int counter, int number, char *input);
void handle_sig(int sig);
void printCommand();
 /*setup() reads in the next command line, separating it into distinct tokens using whitespace as delimiters. 
   setup() sets the args parameter as a null-terminated string 

*/


int setup(char inputBuffer[], char *args[], int *background, int counter, int number) 
{
	int length, // # of characters in the command line 
	i, 	// loop inde for accessing inputBuffer array 
	start, // index where beginning of the next command parameter is 
	ct;  //index of where to place the next parameter into args[] 

	ct = 0; 

// read what the user enters on the command line 

	length = read(STDIN_FILENO, inputBuffer, MAX_LINE);

	//printf("%d\n", length);
	//inputBuffer[length-1] = '\0';
	//printf ("%d\n",inputBuffer[length-1] == '\n');
//printf("%s\n", inputBuffer);
	char *temp;
	if ((inputBuffer[0]== 'r') && (inputBuffer[2] != '\0')) {
		//printf("entered here!!!\n");
		int i; 
		// scan the array in reverse the to 
		//look for the most recent command starting with 'x'
		for (i = 9; i >=0; i--) {
			if (inputBuffer[2] == history[i][0]) {
				//memmove(inputBuffer, history[i], strlen(history[i]));
				strcpy(inputBuffer, history[i]);
				//inputBuffer[strlen(history[i])] = '\0';
				//printf("%s\n", inputBuffer);
				printf("%d ", numComm[i]); 
				printf("%s\n", inputBuffer);
				
				break; //get out of the loop 
			}

		}
		length = strlen(inputBuffer);

	}
	if (inputBuffer[0] == 'r' && inputBuffer[1] == '\n') {
		//printf("Enter here\n");
		strcpy(inputBuffer, history[counter-1]);
		printf("%d ", numComm[counter-1]); 
		printf("%s\n", inputBuffer);
		length = strlen(inputBuffer);
	}




	int tempStatus;
	int index;
	//inputBuffer[length-1] = '\0';
	for (index = 9; index >=0; index--){
		if (history[index][0] == inputBuffer[0] && history[index][1] == inputBuffer[1]) {
			//printf("Error message\n");
			tempStatus = comStatus[index];
			break;
		}
	}

	if (tempStatus != -1) {
		inputBuffer[length] = '\0'; //string manipulation 
		addCommands(counter, number, inputBuffer);
	
	}

	else {
		printf("Invalid command not recorded!\n");
	
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
					inputBuffer[i] = '\0';
				}




		}	
	}
	//printf("%s\n", args[1]);

	args[ct] = NULL; //just in case the input line was > 80 
	

	return tempStatus;
}

//waitpid(child_status, &status,wnohang)  
char directory[MAX_LINE];

int main (void) 
{
	int quantity = 30;
	char inputBuffer[MAX_LINE]; // buffer to hold the command entered 
	int background; // equals 1 if a command is followed by '&'

	char *args[MAX_LINE/+1]; //command line (of 80) has max of 40 arguments 
	

	
	counter = 0;  //initialize the counter 
	number = 1; // first command is numbered 1 
	char array[80];
	while (1) {   //program terminates normally inside setup
		background = 0; 
		printf (" COMMAND -> \n");
		
		

		int tempStatus;
		tempStatus = setup(inputBuffer, args, &background, counter, number);
		
	/*	
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
*/


		if ( tempStatus != -1){
			counter++;
			number++;
		}
		//else {
			
		//}
		
		
 // get next command 
		//addCommands(counter, number, inputBuffer);

		if (strcmp(args[0], "pwd") == 0) {
				if (getcwd(directory, MAX_LINE) != NULL){ 

					printf ("Currently in: %s\n", directory);
				}
			}
		if (strcmp(args[0], "cd") == 0) {
			getcwd(directory, MAX_LINE);
			if (chdir(args[1]) == 0) {
				getcwd(directory, MAX_LINE);
				printf("Changed to : %s\n", directory);
			}
			else printf("Invalid path\n");
		}
		


		int child_status = fork(); 
		int status;
		int execStatus;

		if (child_status == 0) {
			
			if (strcmp ("pwd", args[0]) !=0 && strcmp("history", args[0]) != 0 && strcmp("cd", args[0]) != 0) {
				execStatus = execvp(args[0], args);
				//printf ("%d\n", execStatus);
				//printf("function in child %d\n", strcmp ("pwd", inputBuffer));
			}
				
			//printf ("%d\n", i);
			if ( execStatus == -1) {
				if (strcmp("history", args[0])==0 || strcmp("pwd", args[0])==0 ||
					strcmp("jobs", args[0])==0 || strcmp("fg", args[0])==0 || strcmp("cd", args[0])==0){
					comStatus[counter-1] = 1;
					continue;
				}
				else {
					printf("invalid command status\n");
					comStatus[counter-1] = -1;
					//printf("%d\n", comStatus[counter-1]);
				}	
			}


			
		}
		else {
			
			if (strcmp(args[0], "exit")==0){
				kill(0, SIGKILL);
				exit(1);
			}
			
			if (strcmp(args[0], "history") == 0) {
				printCommand();
				//printf("%s\n", history[2]);
			}

			if ( strcmp(args[0], "history") == 0) {
				
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

	//char str[80];

	//strcpy(str, input);

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

	//printf("\n");

}



