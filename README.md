INSTRUCTION
1. Build server using make command.
2. Run server: ./server

APPROACH
This server is based on poll() function. It can handle many connections without creating a separate thread for each client. Polling mechanism allows to avoid context switching overhead, save resources, which are needed for creating and managing threads. Also this approach is more efficient with I/O bound tasks (waiting for new connections).
This server could be improved if we will use multhithreading approach with CPU-bound operations (like parsing of HTTP message). 
