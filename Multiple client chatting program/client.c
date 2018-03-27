/**
Program to establish the "client side" of a chatting connection between multiple clients via a server through socket programming and multi-threading.
**/

//Created by Emad Bin Abid and Tasneem Adnan on March 27th, 2018

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error_message(char* errorString)
{
	print("[-]Error - %s\n", errorString);
	exit(0);
}

void success_message(char* successString)
{
	print("[+]Success - %s\n", successString);
}

int main(int argc, char* argv[])
{
	/**Command line arguments' validation starts... **/
	if(argc < 3)
	{
		error_message("Too less arguments. Exactly 3 arguments expected.");
	}
	if(argc > 3)
	{
		success_message("Too many arguments. Exactly 3 arguments expected.");
	}
	/**Command line arguments' validation ends! **/
	
	//Initializing command line arguments.
	char* server_ip, server_socket, client_id;
	server_ip = argv[1];
	server_socket = argv[2];
	client_id = argv[3];

	return 0;
}
