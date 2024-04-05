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
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == SOCKET_ERROR) quit("Create socket");
	printf("Socket is ready\n");

	// Set up the local socket address
	SOCKADDR_IN sa, ca;								// sa: server (local) address, ca: client (remote) address // 0.0.0.0 = any address
	sa.sin_addr.s_addr = INADDR_ANY; //ANY ADDRESS CAN BE GIVEN TO THIS HOST
	sa.sin_port = htons(atoi(argv[1]));				// Convert the port number from string to integer // htons changes endiness
	sa.sin_family = AF_INET;

	// Bind the socket to the local address and port
	ret = bind(s, (SOCKADDR*)&sa, sizeof(sa));
	if (ret == SOCKET_ERROR) quit("binding");
	printf("binding is okay\n");
	// have to listen to the socket if binding ok
	ret = listen(s, 5); //5= backlog - size of the queue bet. 5-10 is not busy server
	if (ret == SOCKET_ERROR) quit("listen");
	printf("Servre is lsitening on port %s\n", argv[1]);
	while (1) {
		int clen = sizeof(ca);
		SOCKET cs = accept(s, (SOCKADDR*)&ca, &clen); // TO GET AND ACCEPT A CONNECTION AFTER LISTENING and cs - client socket
		if (cs == INVALID_SOCKET) quit("accept");
		printf("Client %s:%d is connected\n", inet_ntoa(ca.sin_addr), ntohs(ca.sin_port));
		// from here, we should make multi-process programming:
		while (1)  {// run as child process, communicating with the client
			char buf[1000] = { 0 };
			ret = recv(cs, buf, 1000, 0);
			if (ret == SOCKET_ERROR) quit("recv");
			printf("\t Client >> %s\n", buf);
			if (buf[0] == '#') break;
			printf("Return :: ");
			printf(buf, 0, 1000);
			fgets(buf, 1000, stdin);
			ret = send(cs, buf, strlen(buf), 0);
		}
		closesocket(cs); //destroy the client socket
		break;
	}
}
//44440-44450 ports
// 4 way handshake used when 2 parties want to stop the connection