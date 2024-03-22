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
	SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s == SOCKET_ERROR) quit("Create socket");
	printf("Socket is ready\n");
	SOCKADDR_IN sa, ca; //sa is remote socket address
	sa.sin_addr.s_addr = inet_addr(argv[1]); //server ip
	sa.sin_port = htons(atoi(argv[2]));		//server port
	sa.sin_family = AF_INET;
	
	printf("udp client is okay\n");
	while (1) {
		char buf[1000] = { 0 }; //initialize all the 100 bytes to 0
		printf("Client ::");
		fgets(buf, 1000, stdin); // take a line of message
		ret = sendto(s, buf, strlen(buf), 0, (SOCKADDR*)&sa, sizeof(sa));
		if (ret == SOCKET_ERROR) quit("sendto"); // error handling
		memset(buf, 0, 1000);
		ret = recvfrom(s, buf, 1000, 0, NULL, NULL); // 0 is flag and NULL, NULL are pointers if we dont want to know the remote
		if (ret == SOCKET_ERROR) quit("recvfrom"); // error handling
		printf("\tServer >> %s\n", buf);
	}

}
