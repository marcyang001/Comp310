#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>

#define nbReader 10
#define nbWriter 6




sem_t rw_mutex; //global variable shared with reader and writer
sem_t mutex;  // mutual exclusion variable (how many readers)
int read_count = 0;
static int glob = 0; //global content accessed by the reader and writer

int readerIteration;
int writerIteration;


struct waitReader {
	//double waitTime_reader[readerIteration];
	double average;
	double min_waitTime; 
	double max_waitTime; 
};

struct waitWriter {
	//double waitTime_reader[readerIteration];
	double average;
	double min_waitTime; 
	double max_waitTime; 
};


struct waitReader r_wait; 
struct waitWriter w_wait;
//double waitTime_writer[nbWriter];


void *Writer(void *arg) {


	//measure the time here using gettimeofday() 
	struct timeval start, end;

	gettimeofday(&start, NULL);  
    double t1 = start.tv_sec * 1000000 +(start.tv_usec);

	int i;
	int WriterNumber;
	r_wait.min_waitTime = -1;
	r_wait.max_waitTime = -1;
	for (i = 0; i < writerIteration; i++) {
	//do{
		
		int randomTime = rand() % 101000;
		WriterNumber = (int)arg;
		wait(rw_mutex);

		//start of critical section
		int temp = glob; 
		glob = glob + 10; 

		printf("Writer %d increments the value from %d to %d\n", WriterNumber,temp, glob);
		// end of critical section

		//sleep(1);
		usleep(randomTime);
		signal(rw_mutex);
	}		
	gettimeofday(&end, NULL);  
    double t2 = end.tv_sec * 1000000 +(end.tv_usec);  

  // 	r_wait.waitTime_writer[WriterNumber]= t2 - t1;
    

}

void *Reader (void *arg) {

	
	//measure the time here
	struct timeval start, end;
	
	 


	int ReaderNumber;
	int i; 
	r_wait.average = 0;
	r_wait.min_waitTime = -0.1;
	
	//start the timer
	gettimeofday(&start, NULL);
	for (i = 0; i < readerIteration; i++) {


		int randomTime = rand() % 101000;

		ReaderNumber = (int) arg;
 
		wait(mutex); 
		
		gettimeofday(&end, NULL); 

		double newTime = ((end.tv_sec * 1000000 + end.tv_usec)- (start.tv_sec * 1000000 + start.tv_usec));
		r_wait.average = (r_wait.average*(readerIteration) +newTime)/(readerIteration+1);


		read_count++;
		if (read_count ==1) 
			wait(rw_mutex);
		signal(mutex);


		

		//printf("%lf\n", newTime);
		if (i == 0 && (r_wait.min_waitTime <0 )) { //store the first timer 
			r_wait.min_waitTime = newTime;
			
		}
		else if (i == 0 && (r_wait.max_waitTime < 0)){
			r_wait.max_waitTime = newTime;
		}
		else if (i > 0) { //compare the first with the new time
			
			if (newTime < r_wait.min_waitTime) { 
				r_wait.min_waitTime = newTime;
				//printf("Entered here : %lf\n", newTime);
			}
			if (newTime > r_wait.max_waitTime) {
				r_wait.max_waitTime = newTime;
			}

		}
		//r_wait.waitTime_reader[ReaderNumber] = newTime;


		//start of critical section 
		printf("Reader %d is reading the number: %d\n", ReaderNumber,glob);
		//end of critical section		


		usleep(randomTime);
		wait(mutex);
		read_count--; 

		if(read_count ==0) 
			signal(rw_mutex);
		signal(mutex);
	}
	 

	

}




int main(int argc, char *argv[]) {


	pthread_t t_read[nbReader], t_write[nbWriter];
	pthread_t r1, w1;
	int s, i, j; 

	if (argc != 3) {
		printf("Usage: ProgramName [Number-of-Reader-Access] [Number-of-Access]\n");
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
		if (i < nbReader) {
			s = pthread_create(&t_read[i], NULL, Reader, (void *)i);

		}

	}
	// creation of 10 writer threads
	
	for (j = 0; j < nbWriter; j++){
		
		s = pthread_create(&t_write[j], NULL, Writer, (void *)j);
	}


	for (i = 0; i<nbReader; i++) {
		if( i < nbReader)
			s = pthread_join(t_read[i], NULL);
	}

	for (j = 0; j<nbWriter; j++) {
		if(j < nbWriter)
			s = pthread_join(t_write[j], NULL);
	}

	printf("\n");
	printf("\n");
	printf("***********Waiting time of the threads *************\n");
	printf("\n");
	printf("\n");

	printf("Writer threads:\n");
	//for (i = 0; i<nbWriter; i++) {
	//	printf("%.1f microseconds elapsed\n", waitTime_writer[i]);
	//}
	printf("\n");

	printf("Reader threads: \n");

	printf("Average waiting time: %.1f microseconds\n", r_wait.average);
	printf("Minimum waiting time: %.1f microseconds\n", r_wait.min_waitTime);
	printf("Maximum waiting time: %.1f microseconds\n", r_wait.max_waitTime);
	//for (i = 0; i<readerIteration; i++) {
	//	printf("%.1f microseconds elapsed\n", r_wait.waitTime_reader[i]);
	//}


	return 0;

}