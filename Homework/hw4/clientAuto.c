#include<sys/socket.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/wait.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 2119
int main() {
	int n;
	printf("Input query times n:\n");
	scanf("%d",&n);
	pid_t pid;
	pid = fork();
	for(int i = 0; i < n; i++) {
		if(pid == 0) {
			pid = fork();
			if(pid == 0) {
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
			
				char buffer[] = "query";
				write(clientSocket, buffer, strlen(buffer));
				close(clientSocket);
			}else {
				exit(0);
			}
		}else {
			int status;
			wait(&status);
		}
	}
	return 0;
}
