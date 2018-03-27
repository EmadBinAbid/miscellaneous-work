#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>		//POSIX message queue

#include "common.h"


void error(char* error_string)
{
	perror(error_string);
	exit(0);
}

int main(int argc, char **argv)
{
	//File writing and array filling starts...	
	srand(time(NULL));
	int random;
	int resource_bank[1000];
		

	FILE* file = fopen("producer.txt", "w");
	
	if(file != NULL)
	{	
		int i;
		for(i=0; i<1000; i++)
		{	
			random = rand() % 1000;
			resource_bank[i] = random;
			fprintf(file, "%d\n", random);	
		}
		fclose(file);
	}
	else
	{
		printf("ERROR opening file, %s\n", "producer.txt");
	}
	//File writing and array filling ends!	
	
	mqd_t mq;
	char buffer[MAX_SIZE];
	int counter = 0;
	
	/* open the mail queue */
	mq = mq_open(QUEUE_NAME, O_WRONLY);
	if(mq < 0)
	{
		error("ERROR: \"consumer\" creates a message queue.\n");
	}
	else
	{
		printf("Message queue opened successfully.\n");
	}

	printf("\"producer\" initialised.\n");
	printf("Waiting for \"consumer's\" response ...");

	file = fopen("producer.txt", "r");
	if(file != NULL)
	{	
		do
		{
			memset(buffer, 0, MAX_SIZE);
			fgets(buffer, MAX_SIZE, file);

			/* send the message */
			CHECK(0 <= mq_send(mq, buffer, MAX_SIZE, 0));
			counter++;
		}
		while (counter<1000);
		fclose(file);
	}
	else
	{
		printf("ERROR opening file, %s\n", "producer.txt");
	}

	printf("\nSent: 1000 messages.\n");
	printf("\"producer\" exited.\n");
	/* cleanup */
	
	if(mq_close(mq) < 0)
	{
		error("ERROR: Message queue unable to close.\n");
	}
	else
	{
		printf("Message queue successfully closed.\n");
	}
	
	return 0;
}
