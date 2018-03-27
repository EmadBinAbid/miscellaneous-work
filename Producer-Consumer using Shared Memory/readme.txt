1. "consumer.c" creates a shared memory, acts as a client, sends 1000 random integers to the server via a shared memory. 
2. The server side is handled by "producer.c". Server is not allowed to create a shared memory in my implementation. The implementation can be enhanced. 
3. "Makefile" generates the .exe files for client and server.
4. Semaphores are used as synchronization mechanism in "producer.c" and "client.c".
