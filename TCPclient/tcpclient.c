#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <WinSock2.h>


void quit(const char* msg) {
	printf("Error: %s\n", msg);
	exit(1);
}

int main(int argc, char** argv) { //take port number from user ; udpserver.exe 12345
	if (argc != 3) quit("Command argument"); // client also needs server's ip
	WSADATA w;
	int ret = WSAStartup(MAKEWORD(2, 2), &w);
	if (ret == 1) quit("Winsock API");
	//make a socket
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //DATAGRAM SOCKET CHANGES TO STREAM SOCKET
	if (s == SOCKET_ERROR) quit("Create socket");		//SOCKET ADDRESS IS ALWAYS IP AND PORT
	printf("Socket is ready\n");
	//address
	SOCKADDR_IN sa; //sa is remote socket address
	sa.sin_addr.s_addr = inet_addr(argv[1]); //server ip
	sa.sin_port = htons(atoi(argv[2]));		//server port
	sa.sin_family = AF_INET; //ipv4

	printf("TCP client is okay\n");
	// make a connection
	ret = connect(s, (SOCKADDR*)&sa, sizeof(sa)); //if
	if (ret == SOCKET_ERROR) quit("connect");		// i 
	printf("Got the server!\n");					// while loop this part -> SYN Flood
	while (1) {
		//char buf[1000] = { 0 };
		char buf[1000] = { 0 }; //"GET /~gc/index.html HTTP/1.1\r\nHost: ww.cc.puv.fi\r\n\r\n"; // recieve 1000 bytes of data
		printf("Client to send (starting '#' with terminate): ");
		/*printf("Client to send: ");*/
		fgets(buf, 1000, stdin);
		ret = send(s, buf, strlen(buf), 0);
		if (ret == SOCKET_ERROR) quit("send");
		if (buf[0] == '#') break;
		memset(buf, 0, 1000);
		ret = recv(s, buf, 1000, 0);
		if (ret == SOCKET_ERROR) quit("recv");
		printf("\tServer:: %s\n", buf);
	}
	closesocket(s);
}
