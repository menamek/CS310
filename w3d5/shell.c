#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <dirent.h>


#define MAXLINE 80
#define MAXARG 20
#define WHITE " \n\t"


// turn the given command into tokens
void tokenize(char* cmd, char* argv[]) {

	int i=0;
	char* tok = strtok(cmd, WHITE);
	argv[i++] = tok;

	if(i == MAXARG && strtok(NULL, WHITE) != NULL){
		printf("%s","maximum arg limit passed");	
	}


	while (i < MAXARG && tok != NULL) {
		// last tok will be NULL
		argv[i++] = tok = strtok(NULL, WHITE); 
	} 


}

// fork and then exec the tokenized command 
// don't wait for it to finish (run in the background)
// prompt therefore usually re-appears first, then child output
int background(char* argv[]) {
	int i;
	int in;
	int out;
	int out2;
	union wait status;
	int id = fork();

	if (id == 0) {

		for(i=0; argv[i]!=NULL; i++){
			if(strcmp(argv[i],"<")==0){
				argv[i]=NULL;
				in = open(argv[i+1],O_RDONLY);
				dup2(in,STDIN_FILENO);
				close(in);
			} else if(strcmp(argv[i],">")==0){
				argv[i]=NULL;
				out = open(argv[i+1],O_WRONLY|O_CREAT,0666);
				dup2(out,STDOUT_FILENO);
				close(out);

			} else if(strcmp(argv[i],">>")==0){
				argv[i]=NULL;
				out2 = open(argv[i+1],O_WRONLY|O_APPEND,0666);
				dup2(out2,STDOUT_FILENO);
				close(out2);

			} 
		}

		execv(argv[0], argv);
		_exit(1); // if we get here execv failed


	} else if (id < 0) {
		fprintf(stderr, "fork failed\n");
		perror("background");
	}
	
	printf("wait\n");
	id = wait(&status);
	printf("id=%d, status=%d\n",id,  status.w_status);

	return id;

}

void pip(char* argv[]){
	int sep;
	for (sep = 1; argv[sep]!=NULL; sep++) { // start at likely place for sep
		if (strcmp(argv[sep], "|") == 0) {
			argv[sep] = '\0'; // break cmdline into two strings
			break;
		}
	}

	// make pipe
	int fds[2];
	pipe(fds);

	// second child, runs second command, and reads from pipe (it's stdin)
	int pid2;
	if ( (pid2 = fork()) == 0) {
		close(fds[1]);   // close the write end
		dup2(fds[0], 0); // 0 (stdin) becomes duplicate of fds[0]
		close(fds[0]);   // close the read end (stdin from pipe will still work)
		execv(argv[sep+1], &argv[sep+1]); // it's stdin reads from pipe
		_exit(1);        // bad exit, execv failed
	}

	// first child, runs first command, and writes to the pipe (it's stdout)
	int pid1;
	if ( (pid1 = fork()) == 0) {
		close(fds[0]);   // close the read end
		dup2(fds[1], 1); // 1 (stdout) becomes duplicate of fds[1]
		close(fds[1]);   // close the write end (stdout to pipe will still work)
		execv(argv[0], argv); // it's stdout writes to pipe
		_exit(1);        // bad exit, execv failed
	}

	// parent doesn't use pipe
	close(fds[0]);
	close(fds[1]);

	// do I need to wait???
	int status;
	while (wait(&status) != pid2);
	if (status == 0) {
		printf("Child 2 terminated\n");
	} else {
		printf("Child 2 failed\n");
	}

}

void multipip(char* argv[]){
	int   p[2];
	int   pid;
	int   fd = 0;
	int   i=0;

	for (i = 1; argv[i]!=NULL; i++) { // start at likely place for sep
		if (strcmp(argv[i], "|") == 0) {
			argv[i] = '\0'; // break cmdline into two strings
			break;
		}
	}


	while (argv[i] != NULL){
		pipe(p);
		pid = fork();
		if (pid == 0){
			dup2(fd, 0); //change the input according to the old
			if (argv[i + 2] != NULL)
				dup2(p[1], 1);
			close(p[0]);
			execvp(argv[i], &argv[i]);
			_exit(1);
		}else{
			close(p[1]);
			fd = p[0]; // input for the next
			i++;
		}
	}
	wait(NULL);
}

/*
   int ls (char* dir){
   DIR* dp= opendir(dir);
   struct dirent* entry = readdir(dp);
   while (entry != NULL){
   printf("%s\n",entry->d_name);
   entry=readdir(dp);
   }
   closedir(dp);
   }
 */
// main loop of the shell
int main() {
	char cmd[MAXLINE+1];
	char* argv[MAXARG+1];
	char* got;
	int i;
	int c=0;
	int j;

	while (1) {
		printf("mysh ready$ ");
		got = fgets(cmd, MAXLINE, stdin);
		if (got == NULL) { // user pressed ^D
			printf("\n");
			exit(0); 
		}
	


		tokenize(cmd, argv);

		for (j = 1; argv[j]!=NULL; j++) {

		if(strcmp(argv[J-2],"&")==0){
				argv[i]=NULL;

				return 0;		
		} 

		}


		for (i = 1; argv[i]!=NULL; i++) { // start at likely place for sep
			if (strcmp(argv[i], "|") == 0) {

				c++;
				if (c < 2){

					pip(argv);

					}else{

						multipip(argv);

					}
			}

			background(argv);
		}

	}
}


