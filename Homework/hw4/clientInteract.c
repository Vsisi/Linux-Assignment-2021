#include<sys/socket.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 2119
int main() {
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serverAddr; memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	serverAddr.sin_port = htons(SERVER_PORT);
	
	int res = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(res == -1) {
		printf("Connect Failed");
		return -1;
	}

	char buffer[1000];

	while(1) {
		printf("Input Message, quit to quit:\n");
		scanf("%s", buffer);

		write(clientSocket, buffer, strlen(buffer));
		
		if(strcmp(buffer, "quit") == 0) {
			break;
		}

		printf("Send Successfully\n");
	}
	close(clientSocket);
	return 0;
}
