#include<unistd.h>
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<sys/types.h>
#include<string.h>
#include<stdlib.h>
struct DataFrame{
	int fromPid;
	char data[1000];
	time_t sentTime;
};
int main() {
	int p1[2], p2[2];
	int fatherRead, fatherWrite;
	int childRead, childWrite;
	pid_t pid;
	int pipe1 = pipe(p1), pipe2 = pipe(p2);
	if(pipe1 == -1 || pipe2 == -1) { //pipe creating error
		printf("Pipe Error!");
		return -1;
	}
	fatherRead = p1[0];
	childRead = p2[0];
	fatherWrite = p2[1];
	childWrite = p1[1];
	pid = fork();
	if(pid == -1) {
		printf("Fork Error!");
		return -1;
	}
	else if(pid != 0) { //father
		char str[1000];
		while(1) {
			memset(str, 0, sizeof(str));
			printf("Please input message\n");	
			scanf("%s", str);
			struct DataFrame dataFrame;
		        dataFrame.fromPid = getpid(); strcpy(dataFrame.data, str); time(&dataFrame.sentTime); //send info
			write(fatherWrite, &dataFrame, sizeof(dataFrame));
			if(strcmp(str, "quit") == 0) {
				break;
			}

			struct DataFrame confirmFrame;
			read(fatherRead, &confirmFrame, sizeof(confirmFrame)); //receive ACK
			printf("Received ACK from child process %d at timestamp %ld.\n\n", confirmFrame.fromPid, confirmFrame.sentTime);
		}
		close(fatherWrite);
		close(fatherRead);
		printf("Father Process %d Ended\n", getpid());
	}else { //child
		while(1) {
			struct DataFrame dataFrame;
			read(childRead, &dataFrame, sizeof(dataFrame)); //receive info
			if(strcmp(dataFrame.data, "quit") == 0) {
				break;
			}
			printf("Received data \"%s\" from father process %d at timestamp %ld.\n", dataFrame.data, dataFrame.fromPid, dataFrame.sentTime);

			struct DataFrame confirmFrame; memset(confirmFrame.data, 0, sizeof(confirmFrame.data));
			confirmFrame.fromPid = getpid(); time(&confirmFrame.sentTime); //send ACK
			write(childWrite, &confirmFrame, sizeof(confirmFrame));
		}
		close(childRead);
		close(childWrite);	
		printf("Child Process %d Ended\n", getpid());
	}
	return 0;
}
