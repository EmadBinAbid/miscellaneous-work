#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <mqueue.h>

#include "common.h"

void error(char* error_string)
{
	perror(error_string);
	exit(0);
}

int main(int argc, char **argv)
{
	mqd_t mq;
	struct mq_attr attr;
	char buffer[MAX_SIZE + 1];
	int must_stop = 0;

	/* initialize the queue attributes */
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = MAX_SIZE;
	attr.mq_curmsgs = 0;

	/* create the message queue */
	mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);
	if(mq < 0)
	{
		error("ERROR: Message queue unable to open.\n");
	}
	else
	{
		printf("Message queue opened successfully.\n");
	}

	printf("\"consumer\" initialised.\n");
	printf("Waiting for \"producer's\" response ...");

	int counter = 0;

	FILE* file = fopen("consumer.txt", "w");	
		
	do
	{
		ssize_t bytes_read;

		/* receive the message */
		bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL);
		CHECK(bytes_read >= 0);

		buffer[bytes_read] = '\0';
		counter++;
	
		if(file != NULL)
		{		
			fprintf(file, "%s", buffer);			
		}
		else
		{
			printf("ERROR opening file, %s\n", "consumer.txt");
		}
		//File writing and array filling ends!
	}
	while (counter < 1000);
	fclose(file);
	
	printf("\nReceived: 1000 messages.\n");
	printf("\"consumer\" exited.\n");

	/* cleanup */
	if(mq_close(mq) < 0)
	{
		error("ERROR: Message queue unable to close.\n");
	}
	else
	{
		printf("Message queue successfully closed.\n");
	}
	CHECK((mqd_t)-1 != mq_unlink(QUEUE_NAME));

	return 0;
}
