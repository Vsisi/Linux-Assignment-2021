#include<stdio.h>
#include<unistd.h>
int main() {
	pid_t pid;
	pid = fork();
	if(pid == 0) {
		int child = 0;
		while(1) {
			child++;
			child--;
		}

	}else {
		int a = 0;
		while(1) {
			a++;
			a--;
			a*=5;
			a/=10;
		}
	}
	return 0;
}
