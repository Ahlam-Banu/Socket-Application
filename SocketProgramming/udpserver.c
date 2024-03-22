#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <WinSock2.h>


void quit(const char* msg) {		// Function to print error message and exit
	printf("Error: %s\n", msg);
	exit(1);
}

int main(int argc, char** argv) {				// Main function, entry point of the program, take port number from user ; udpserver.exe 12345
	if (argc != 2) quit("Command argument");	// Check if the number of command line arguments is correct
	// Initialize Winsock
	WSADATA w;
	int ret = WSAStartup(MAKEWORD(2, 2), &w);	// Start Winsock 2.2
	if (ret == 1) quit("Winsock API");
	// Create a socket
	SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s == SOCKET_ERROR) quit("Create socket");
	printf("Socket is ready\n");

	// Set up the local socket address
	SOCKADDR_IN sa, ca;								// sa: server (local) address, ca: client (remote) address // 0.0.0.0 = any address
	sa.sin_addr.s_addr = INADDR_ANY;
	sa.sin_port = htons(atoi(argv[1]));				// Convert the port number from string to integer // htons changes endiness
	sa.sin_family = AF_INET;

	// Bind the socket to the local address and port
	ret = bind(s, (SOCKADDR*)&sa, sizeof(sa));
	if (ret == SOCKET_ERROR) quit("binding");
	printf("binding is okay\n");
	while (1) {
		char buf[1000] = { 0 };			// Initialize buffer to store received data	//initialize all the 100 bytes to 0
										// memset(buf, 0, 1000) <- another way
		int rlen = sizeof(ca);

		// Receive data from client
		ret = recvfrom(s, buf, 1000, 0, (SOCKADDR*)&ca, &rlen);	// 0 is flag and NULL, NULL are pointers if we dont want to know the remote
		if (ret == SOCKET_ERROR) quit("recvfrom");				// error handling
		
		// Print client address, port, and received message
		printf("Client (%s : %d): %s\n", inet_ntoa(ca.sin_addr), ntohs(ca.sin_port), buf);
	}
}