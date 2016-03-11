nclude <stdio.h>
#include <unistd.h>
#include <string.h>
#define MAXLINE 80
#define MAXARG 20
#define WHITE " \n\t"


void background(char* cmd){
	char* argv[MAXARG+1];
	int i = 0;
	char* tok = strtok(cmd, WHITE);
	argv[i++] = tok;
	while( i < MAXARG && tok != NULL){
		argv[i++] = tok = strtok(NULL, WHITE);
	}
	int id = fork();
	if (id == 0){
		execv(argv[0], argv);
		_exit(1);
	} else if (id < 0){
		fprintf(stderr, "fork failed\n");
		perror("background");
	} else {
		wait(NULL);
	}

}

int main(){
	char cmd[MAXLINE];
	while(1){
		printf("Mysh ready$ ");
		fgets(cmd, MAXLINE-1, stdin);
		background(cmd);
	}
}
