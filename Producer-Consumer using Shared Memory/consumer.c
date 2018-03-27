#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <time.h>

#define BUFFER_SIZE 10
#define SEM_NAME "/semaphore_example"

void error(char* error_string)
{
	perror(error_string);
	exit(0);
}

typedef struct
{
	int counter;
	int in;
	int out;
	int buffer[BUFFER_SIZE];
} shm_struct;

int main(int argc, char* argv[])
{
	int shmid;
	key_t key;
	shm_struct * shm;
	int max_vals;
	int num_vals;
	sem_t *semaphore = sem_open(SEM_NAME, O_CREAT, 0666, 1);
    
	FILE* file;    
	file = fopen("consumer.txt","w");
    
	/*
	* We'll name our shared memory segment
	* "5678".
	*/
    
	key = 5670;

	/*
    	* Create the segment.
    	*/
   	if ((shmid = shmget(key, sizeof(shm_struct), IPC_CREAT | 0666)) < 0)
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
	* Now put some things into the memory for the
	* other process to read.
	*/
	shm->counter=0;
	shm->in = 0;
	shm->out = 0;
   	num_vals = 0;
   	srand(time(NULL));
	
	printf("Reading server's data ...\n");

    	while (num_vals < 1000)
    	{
		while (shm->counter==BUFFER_SIZE)
			; //wait


		shm->buffer[shm->in] = rand()%1000;
		fprintf(file, "%d\n", shm->buffer[shm->in]);

		shm->in = (shm->in+1)%BUFFER_SIZE;

		
		//shm->counter++;
		/*
		 * simulate the increment in steps to create race condition.
		 */
		sem_wait(semaphore);
		int tmp = shm->counter; 
		usleep (100);
		tmp++;
		usleep (100);
		shm->counter = tmp;
		sem_post(semaphore);
		num_vals++;
   	}

   	/*
     	* Finally, we wait until the other process 
	* changes the first character of our memory
	* to '*', indicating that it has read what 
	* we put there.
	*/
  	while (shm->counter != 0)
     		sleep(1);

   	/*
    	* cleanup: detach and remove shm
    	*/
   	fclose(file);
	
	printf("Data read successfully.\n");	

   	shmdt (shm);
   	shmctl (shmid, IPC_RMID, NULL);
    	sem_close(semaphore);
	printf("Shared memory successfully released.\n");
    	exit(0);
}

