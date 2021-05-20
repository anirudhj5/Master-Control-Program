#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>
#include "string_parser.h"






typedef unsigned long old_sigset_t; /* at least 32 bits */ 




void script_print (pid_t* pid_ary, int size, int sig);


void signaller(pid_t* pid_arr, int n, int sig){
	sleep(2);
	for(int i = 0; i < n; i++){
		printf("Parent process: %d - Sending signal: %d to childprocess: %d\n", getpid(), sig, pid_arr[i]); 
		kill(pid_arr[i], sig);  
	}

}



int main(int argc,char*argv[])
{
	
	/*
	*	TODO
	*	#1	declear child process pool
	*	#2 	spawn n new processes
	*		first create the argument needed for the processes
	*		for example "./iobound -seconds 10"
	*	#3	call script_print
	*	#4	wait for children processes to finish
	*	#5	free any dynamic memories
	*/

	if (argc != 2)
	{
		printf ("Wrong number of arguments\n");
		exit (0);
	}

	int n = 0;
	char c;

	FILE *fp;
	fp = fopen(argv[1], "r");
	if(fp == NULL){
		printf("Error: Could Not Open File\n");
		return 0;
	}


	for (c = getc(fp); c != EOF; c = getc(fp)){
        if (c == '\n'){ // Increment count if this character is newline
            n++;
		}
	}
	n++;
	fclose(fp);
  
	fp = fopen(argv[1], "r");

	size_t len = 0;
	char *lineptr;
	command_line small_token_buffers[n];
	int y = 0;
	while(getline(&lineptr, &len, fp) != -1){
		small_token_buffers[y] = str_filler(lineptr, " ");
		y++;
	}

	int sig;
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigprocmask(SIG_BLOCK, &set, NULL);

	pid_t *pid_arr = malloc(sizeof(pid_t)*n);
	for(int i = 0; i < n; i++){
		pid_arr[i] = fork();
		if(pid_arr[i] < 0){
			perror("Fork Error");
			exit(1);
		}else if(pid_arr[i] == 0){
			
			printf("WAITING FOR SIGUSR1 SIGNAL\n");
			sigwait(&set, &sig);
			printf("RECEIVED SIGUSR1 SIGNAL\n");
			if(execvp(small_token_buffers[i].command_list[0], small_token_buffers[i].command_list) == -1){
				perror("Error: ");
				exit(1);
			}
			exit(-1);
		}else{ 
			/*
			signaller(pid_arr, n, SIGSTOP);
			sleep(5);
			signaller(pid_arr, n, SIGCONT);
			sleep(3);
			signaller(pid_arr, n, SIGINT);
			*/
		}
	}
	sleep(5);
	signaller(pid_arr, n, SIGUSR1);
	signaller(pid_arr, n, SIGSTOP);
	sleep(5);
	signaller(pid_arr, n, SIGCONT);
	sleep(3);
	signaller(pid_arr, n, SIGINT);
	fclose(fp);
	free(pid_arr);
	return 0;
}