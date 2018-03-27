/*
 * shm-client - client program to demonstrate shared memory.
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>


#define BUFFER_SIZE 10
#define SEM_NAME "/semaphore_example"

void error(char* error_string)
{
	perror(error_string);
	exit(0);
}

typedef struct {
	int counter;
	int in;
	int out;
	int buffer[BUFFER_SIZE];
} shm_struct;


int main(int argc, char* argv[])
{
   	int shmid;
   	key_t key;
   	shm_struct *shm;
   	int max_vals;
   	int num_vals;
   	int *recvd_vals;
  	sem_t *semaphore = sem_open(SEM_NAME, 0);
    
	FILE* file;
   	file = fopen("producer.txt","w");

   	/*
    	* We need to get the segment named
   	* "5670", created by the consumer.
    	*/
   	key = 5670;

   	/*
    	* Locate the segment.
    	*/
    	if ((shmid = shmget(key, sizeof(shm_struct), 0666)) < 0)
	{
        	error("ERROR: Failed to access shared memory.\n");
    	}
	else
	{
		printf("Shared memory accessed successfully.\n");
	}

   	/*
    	* Now we attach the segment to our data space.
    	*/
   	if ((shm = shmat(shmid, NULL, 0)) == (shm_struct *) -1)
	{
        	error("ERROR: Failed to attach shared memory.\n");
   	}
	else
	{
		printf("Shared memory attached successfully.\n");
	}

   	/*
    	* Now read what the server put in the memory.
    	*/
   	num_vals =0;
	printf("Writing to \"shm\" ...\n");
   	while (num_vals != 1000)
   	{
		while (shm->counter==0)
		{
			
		}//wait

		int val_consumed = shm->buffer[shm->out];
		fprintf(file, "%d\n", shm->buffer[shm->out]);
		shm->out = (shm->out+1)%BUFFER_SIZE;
		sem_wait(semaphore);
		shm->counter--;
		
		sem_post(semaphore);
		num_vals++;
   	}

   	shm->counter = 0;
	
	printf("Data written successfully.\n");

   	/*
    	* cleanup
    	*/
   	fclose(file);
   	shmdt (shm);
   	sem_close(semaphore);
	printf("Shared memory successfully released.\n");
   	exit(0);
}
