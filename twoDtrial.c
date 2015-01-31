#include <stdio.h> 
#include <string.h>

char history[10][80]; //store the 10 previous commands 
int numComm[10]; //store the command number 

int counter; // index of the array
int number;   // command number 


void printCommand();
void addCommands(int counter, int number, char *input);
void executeFunctions(char *argument);


int main() {

	
	char input[80];  //
	char args[80]; //temp command storage

	counter = 0;  //initialize the counter 
	number = 1; // first command is numbered 1 

	while (1) {

		// user type in the command 
		fgets(input, 80, stdin);
		
		//exit command 
		if (strcmp(input, "exit\n") == 0){
			break;
		}

		else {
			// identify r l or r 'x' command
			if ((input[0]== 'r') && (input[2] != '\0')) {
		
					int i; 
					// scan the array in reverse the to 
					//look for the most recent command starting with 'x'
					for (i = 9; i >=0; i--) {
						// if the command is found 
						if (input[2] == history[i][0]) {
							//copy into a temp memory 
							strcpy(args, history[i]);
							//print the number and the command
							printf("%d ", numComm[i]); 
							printf("%s\n", history[i]);
							//add to the history 
							addCommands(counter, number, args);
							counter++; //increment the index 
							number++;	//increment the number 

							break; //get out of the loop 
						}
					}
					
					//execute in the real shell here 
					executeFunctions(args);
			}
			// identify the command r\n
			else if (strcmp(input, "r\n")==0) {
				//copy to the temp memory. 
				//counter - 1 means to print the last added command 
				// every time you add the command, you increment 
				strcpy(args, history[counter-1]);
				printf("%d ", numComm[counter-1]);
				printf("%s\n", history[counter-1]);
				addCommands(counter, number, args);
				counter++; 
				number++;

				//execute in the real shell
				executeFunctions(args);
			}
			// 
			else {
				strcpy(args, input);
				// add command to the history
				addCommands(counter, number, args);
				counter++; 
				number++;
				
				//execute in the real shell 
				executeFunctions(args);
			}


			
		}
		
	}





	printf("--------------------------\n");

/*
	while(1) {

		fgets(args, 80, stdin);
		if (strcmp(args, "exit\n") == 0){
			break;

		}
		if(strcmp(args, "print\n") == 0) {

			printCommand();
		}

		if (args[0]== 'r' && args[2] != '\0') {
			int i; 
			for (i = 9; i >=0; i--) {
				if (args[2] == history[i][0]) {
					printf("%d ", numComm[i]);
					printf("%s", history[i]);
					break;
				}
			}

		}



	}
*/

	//printCommand();
}

void executeFunctions(char *argument){
	
	if(strcmp(argument, "print\n") == 0) {

		printCommand();
	}

}

void addCommands(int counter, int number, char *input) {

	if (counter < 10) {
				numComm[counter] = number;
				strcpy(history[counter], input);
			}
			else {
				int i; 
				for (i = 0; i < 9; i++) {
					*(numComm + i) = *(numComm+i+1);
					strcpy(history[i], history[i+1]);
				}
				numComm[9] = number;
				strcpy(history[9], input);
			}


}


void printCommand() {

	int i;
	for (i = 0; i< 10; i++) {
		printf("%d ", numComm[i]);
		printf("%s", history[i]);
	}
	printf("\n");

}