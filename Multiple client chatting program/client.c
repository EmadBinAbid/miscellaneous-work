/**
Program to establish the "client side" of a chatting connection between multiple clients via a server through socket programming and multi-threading.
**/

//Created by Emad Bin Abid and Tasneem Adnan on March 27th, 2018

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void error_message(char* errorString)
{
	printf("[-]Error - %s\n", errorString);
	exit(0);
}

void success_message(char* successString)
{
	printf("[+]Success - %s\n", successString);
}

int main(int argc, char* argv[])
{
	/**Command line arguments' validation starts... **/
	if(argc < 4)
	{
		error_message("Too less arguments. Exactly 3 arguments expected.");
	}
	if(argc > 4)
	{
		error_message("Too many arguments. Exactly 3 arguments expected.");
	}
	/**Command line arguments' validation ends! **/
	
	//Initializing command line arguments.
	char* server_ip = argv[1];
	int server_port = atoi(argv[2]);
	char* client_id = argv[3];
	
	int server_socket;
	struct sockaddr_in server;
	char client_message[1024], server_reply[1024];

	//Creating socket.
	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	if(server_socket < 0)
	{
		error_message("Unable to create socket.");
	}
	success_message("Socket created.");	
	
	server.sin_addr.s_addr = inet_addr(server_ip);
	server.sin_family = AF_INET;
	server.sin_port = htons(server_port);
	
	//Connecting to server.
	if(connect(server_socket, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		error_message("Unable to connect with server at given port");
	}
	success_message("Connection with server established.");
	
	//Communicating with server
	while(strcmp(client_message, "/quit") != 0)
	{
		printf("Client: \t");
		scanf("%s", client_message);
		
		//Sending data to server
		if(send(server_socket, client_message, strlen(client_message), 0) < 0)
		{
			error_message("Unable to send message to server.");
		}
		success_message("Message sent.");
		
		//Receiving reply from server
		if(recv(server_socket, server_reply, strlen(server_reply), 0) < 0)
		{
			error_message("Unable to receive message from server.");
		}
		printf("Server: \t%s\n", server_reply);
	}
	close(server_socket);
	return 0;
}
