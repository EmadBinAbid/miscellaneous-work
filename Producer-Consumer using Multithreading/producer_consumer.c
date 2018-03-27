#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

//Global definitions
int COMMUNICATOR = -1; //decides between producer and consumer threads
int value[10];	//current value to be written at consumer end

int producer_array[1000];	//stores values to be written
int producer_index = 0;	

int consumer_array[1000];	//stores values that are received
int consumer_index = 0;

//Function prototypes
void error(char*);
void* producer();
void* consumer();

int main(int argc, char* argv[])
{
	srand(time(NULL));
	int random;

	//File writing and array filling starts..
	FILE* file = fopen("producer.txt", "w");
	
	if(file != NULL)
	{	
		int i;
		for(i=0; i<1000; i++)
		{	
			random = rand() % 1000;
			producer_array[i] = random;
			fprintf(file, "%d\n", random);
		}
		fclose(file);
	}
	else
	{
		printf("ERROR opening file, %s\n", "producer.txt");
	}
	//File writing and array filling ends!
		
	
	pthread_t thread_1, thread_2;

	pthread_create(&thread_1, NULL, producer, NULL);
	pthread_create(&thread_2, NULL, consumer, NULL);

	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
	
	file = fopen("consumer.txt", "w");
	
	if(file != NULL)
	{	
		int i;
		for(i=0; i<1000; i++)
		{	
			fprintf(file, "%d\n", consumer_array[i]);
		}
		fclose(file);
	}
	else
	{
		printf("ERROR opening file, %s\n", "consumer.txt");
	}
	//File writing and array filling ends!
	
	printf("Sent: 1000 messages.\n");
	printf("Received: 1000 messages.\n");
		
	return 0;
}


void error(char* error_string)
{
	perror(error_string);
	exit(0);
}

void* producer()
{
	printf("Reading values ...\n");
	while(producer_index < 1000)
	{
		if(COMMUNICATOR == -1)
		{
			int i;
			for(i=0; i<10; i++)
			{
				value[i] = producer_array[producer_index];
				producer_index++;
			}
			COMMUNICATOR = 0;
		}		
	}
}

void* consumer()
{
	printf("Transferring values ...\n");
	while(consumer_index < 1000)
	{
		if(COMMUNICATOR == 0)
		{
			int j;
			for(j=0; j<10; j++)
			{
				consumer_array[consumer_index] = value[j];
				consumer_index++;
			}
			COMMUNICATOR = -1;
		}		
	}
}

