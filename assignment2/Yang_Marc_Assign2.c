#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>

#define nbReader 100
#define nbWriter 10




sem_t rw_mutex; //global variable shared with reader and writer
sem_t mutex;  // mutual exclusion variable (how many readers)
int read_count = 0;
static int glob = 0; //global content accessed by the reader and writer

int readerIteration;
int writerIteration;

struct waitReader {
	double average_waitTime[nbReader];
	double min_waitTime[nbReader]; 
	double max_waitTime[nbReader]; 
};

struct waitWriter {
	double average_waitTime[nbWriter];
	double min_waitTime[nbWriter]; 
	double max_waitTime[nbWriter]; 
};


struct waitReader r_wait; 
struct waitWriter w_wait;



void *Writer(void *arg) {


	//measure the time here using gettimeofday() 
	struct timeval start, end;

	

	int i = 0;
	int WriterNumber = (int)arg;
	
	double average;
	double newTime;
	w_wait.average_waitTime[WriterNumber] = 0;
	


	//for (i = 0; i < writerIteration; i++) {
	do{
		
		int randomTime = rand() % 100001; //generate from 0 to 100 milliseconds
		
		//start timer 
		gettimeofday(&start, NULL);  
		wait(rw_mutex);

		//end timer 
		gettimeofday(&end, NULL); 
		



		//start of critical section
		int temp = glob; 
		glob = glob + 10; 

		printf("Writer %d increments the value from %d to %d\n", WriterNumber,temp, glob);
		// end of critical section


		//sleep(1);
		usleep(randomTime);
		signal(rw_mutex);

		newTime = ((end.tv_sec * 1000000 + end.tv_usec)- (start.tv_sec * 1000000 + start.tv_usec));
		
		average = (average*(i) +newTime)/(i+1);

		//find max and min time 
		if( i == 0 ) { //first time when i = 0, store any value; 
			w_wait.min_waitTime[WriterNumber] = newTime;
			w_wait.max_waitTime[WriterNumber] = newTime;
		}
		else { //else start comparing when i > 0
			if(newTime < w_wait.min_waitTime[WriterNumber]) {
				w_wait.min_waitTime[WriterNumber] = newTime;
			}
			if (newTime > w_wait.max_waitTime[WriterNumber]) {
				w_wait.max_waitTime[WriterNumber] = newTime;
			}
		}

		//store the addition of all the wait time from each access of a thread
		//w_wait.average_waitTime[WriterNumber] += newTime;

		


		i++;
	}
	while (i < writerIteration);

	//store the average of X number of access of each thread 
	//w_wait.average_waitTime[WriterNumber] = w_wait.average_waitTime[WriterNumber]/writerIteration; 
	w_wait.average_waitTime[WriterNumber] = average;


}

void *Reader (void *arg) {

	
	//measure the time here
	struct timeval start, end;
	
	 


	int ReaderNumber = (int) arg;
	int i = 0; 
	
	double newTime1; 
	double newTime2;
	double totalWait;
	double average;
	r_wait.average_waitTime[ReaderNumber] = 0;
	
	do {


		int randomTime = rand() % 100001; //generate 0 to 100 milliseconds

		//ReaderNumber = (int) arg;
 		
 		//start the timer
		gettimeofday(&start, NULL);
		
		wait(mutex); 
		//end the timer
		gettimeofday(&end, NULL); 

		newTime1 = ((end.tv_sec * 1000000 + end.tv_usec)- (start.tv_sec * 1000000 + start.tv_usec));

		read_count++;
		if (read_count ==1) 
			wait(rw_mutex);
		signal(mutex);


		//start of critical section 
		printf("Reader %d is reading the number: %d\n", ReaderNumber,glob);
		//end of critical section		



		usleep(randomTime);
		
		//start the timer
		gettimeofday(&start, NULL);
		
		wait(mutex); 
		//end the timer
		gettimeofday(&end, NULL); 
		newTime2 = ((end.tv_sec * 1000000 + end.tv_usec)- (start.tv_sec * 1000000 + start.tv_usec));


		//second Critical section 
		read_count--; 
		//out of critical section 

		//find min and max 
		if(read_count ==0) 
			signal(rw_mutex);
		signal(mutex);

		totalWait = newTime1 + newTime2;
		
		//printf("HERE is %d\n", i);
		//r_wait.average_waitTime[ReaderNumber] += totalWait;
		average = (average*(i) +newTime1 + newTime2)/(i+1);

		if( i == 0) {
			//r_wait.average_waitTime[ReaderNumber] = totalWait;
			r_wait.min_waitTime[ReaderNumber] = totalWait;
			r_wait.max_waitTime[ReaderNumber] = totalWait;
		}
		else {
			if(totalWait < r_wait.min_waitTime[ReaderNumber]) {
				r_wait.min_waitTime[ReaderNumber] = totalWait;
			}
			if (totalWait > r_wait.max_waitTime[ReaderNumber]) {
				r_wait.max_waitTime[ReaderNumber] = totalWait;
			}
		}
		
		i++;
		
		//
		

	} while (i < readerIteration);
	if (readerIteration == 0) {
		readerIteration = 1;
	}

	//r_wait.average_waitTime[ReaderNumber] = r_wait.average_waitTime[ReaderNumber]/readerIteration;
	r_wait.average_waitTime[ReaderNumber] = average;

	

}




int main(int argc, char *argv[]) {


	pthread_t t_read[nbReader], t_write[nbWriter];
	pthread_t r1, w1;
	int s, i, j; 

	if (argc != 3) {
		printf("Usage: ProgramName [Number-of-Reader-Access] [Number-of-Writer-Access]\n");
		exit(0);
	}

	readerIteration = atoi(argv[1]);
	writerIteration = atoi(argv[2]);

	//initialize semaphore rw_mutex = 1 
  	if (sem_init(&rw_mutex, 0, 1) == -1) {
    	printf("Error, init semaphore\n");
    	exit(1);
  	}

  	//initialize semaphore mutex = 1 
  	if (sem_init(&mutex, 0, 1) == -1) {
    	printf("Error, init semaphore\n");
    	exit(1);
  	}

  	//creation of 100 reader threads 
	for (i = 0; i < nbReader; i++) {
		s = pthread_create(&t_read[i], NULL, Reader, (void *)i);
	}
	// creation of 10 writer threads
	
	for (j = 0; j < nbWriter; j++){
		
		s = pthread_create(&t_write[j], NULL, Writer, (void *)j);
	}


	for (i = 0; i<nbReader; i++) {
		s = pthread_join(t_read[i], NULL);
	}

	for (j = 0; j < nbWriter; j++) {
		s = pthread_join(t_write[j], NULL);
	}

	printf("\n");
	printf("\n");
	printf("***********Waiting time of the threads *************\n");
	printf("\n");
	printf("\n");


	printf("Writer threads:\n");
	
	double averageWriterThread = 0; 
	for (j = 0; j < nbWriter; j++) {
		averageWriterThread += w_wait.average_waitTime[j];
	} 
	averageWriterThread = averageWriterThread/nbWriter;
	printf("Average waiting time: %.1f microseconds\n", averageWriterThread);
	
	double maxWaitWriter = w_wait.max_waitTime[0];
	for (j = 0; j < nbWriter; j++) {
		if(w_wait.max_waitTime[j] > maxWaitWriter) {
			maxWaitWriter = w_wait.max_waitTime[j];
		}
		
	}

	printf("Maximum waiting time: %.1f microseconds\n", maxWaitWriter);

	double minWaitWriter = w_wait.min_waitTime[0];
	for (j = 0; j < nbWriter; j++) {
		if (w_wait.min_waitTime[j] < minWaitWriter) {
			minWaitWriter = w_wait.min_waitTime[j];
		}
	}
	printf("Minimum waiting time: %.1f microseconds\n", minWaitWriter);
//******************************************************************/

	printf("\n");

	printf("Reader threads: \n");
	
	double averageReaderThread = 0;
	
	for (i = 0; i < nbReader; i++) {
		averageReaderThread += r_wait.average_waitTime[i];
	}
	averageReaderThread = averageReaderThread/nbReader;
	printf("Average waiting time: %.2f microseconds\n", averageReaderThread);
	

	double maxWaitReader = r_wait.max_waitTime[0]; 
	for (i = 0; i < nbReader; i++) {
		if (r_wait.max_waitTime[i] > maxWaitReader) {
			maxWaitReader = r_wait.max_waitTime[i];
		}
	}

	printf("Maximum waiting time: %.2f microseconds\n", maxWaitReader);

	double minWaitReader = r_wait.min_waitTime[0]; 
	for (i = 0; i < nbReader; i++) {
		if (r_wait.min_waitTime[i] < minWaitReader) {
			minWaitReader = r_wait.min_waitTime[i];
		}
	}
	printf("Minimum waiting time: %.2f microseconds\n", minWaitReader);


	return 0;

}
