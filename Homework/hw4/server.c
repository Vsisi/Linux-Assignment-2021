#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<time.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 2119

int main() {
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serverAddr; memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	serverAddr.sin_port = htons(SERVER_PORT);

	int bnd = bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if (bnd == -1) {
		printf("BIND ERROR!");
		return -1;
	}

	listen(serverSocket, 10);
	time_t startTime, nowTime;
	time(&startTime);
	int queryTimes = 0;

	while(1) {
		struct sockaddr_in clientAddr;
		int clientPort;
		int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientPort);
		queryTimes++;
		time(&nowTime);
		double avg = nowTime > startTime ? 1.0 * queryTimes / (nowTime - startTime) : 0;
		printf("Total %d query(s) received from timestamp %ld to %ld, average %.2lf query per sec\n", queryTimes, startTime, nowTime, avg);
		pid_t pid;
		pid = fork();
		if(pid == 0) { //child Process
			pid = fork();
			if(pid == 0) {  //grand Process
				while(1) {
					char buffer[1000]; memset(buffer, 0, sizeof(buffer));
					int readNum;
					readNum = read(clientSocket, buffer, sizeof(buffer));
					if(strcmp(buffer, "quit") == 0 || readNum == 0) {
						break;
					}
					printf("Received data \"%s\", totally %d byte(s)\n", buffer, readNum);

				}
				printf("Server Child Process %d Ended.\n", getpid());
				exit(0);
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
