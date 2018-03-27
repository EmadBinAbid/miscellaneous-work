1. "common.h" shares common variables between "producer.c" and "consumer.c".
2. "consumer.c" creates a message queue, acts as a client, sends 1000 random integers to the server. 
3. The server side is handled by "producer.c". Server is not allowed to create a message queue in my implementation. The implementation can be enhanced. 
4. "Makefile" generates the .exe files for client and server.
