#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>
#include "string_parser.h"


volatile sig_atomic_t alarmCall = 0;


int count = 0;


typedef unsigned long old_sigset_t; /* at least 32 bits */ 


int sum(int* arr, int n){
	int s = 0;
	for(int i = 0; i < n; i++){
		s += arr[i];
	}
	return s;
}

void script_print (pid_t* pid_ary, int size, int sig);

void alarm_handler(int sig){
	if(sig == SIGALRM){
		alarmCall = 1;
		printf("SIGALARM\n");
	}else if(sig == SIGSTOP){
		printf("%d\n", sig);
	}else if(sig == SIGCONT){
		printf("%d\n", sig);
	}
}

void signaller(pid_t* pid_arr, int n, int sig, int i){

	if(i == n+1){
		for(int j = 0; j < n; j++){
			printf("Parent process: %d - Sending signal: %d to childprocess: %d\n", getpid(), sig, pid_arr[j]); 
			kill(pid_arr[j], sig);
		}
	}else{
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
	fclose(fp);

	int child_procs[n];
  
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
	//sigaddset(&set, SIGALRM);
	sigprocmask(SIG_BLOCK, &set, NULL);

	for(int i = 0; i < n; i++){
		child_procs[i] = 0;
	}


	pid_t *pid_arr = malloc(sizeof(pid_t)*n);
	for(int i = 0; i < n; i++){
		pid_arr[i] = fork();
		if(pid_arr[i] < 0){
			perror("Fork Error: ");
			exit(1);
		}else if(pid_arr[i] == 0){
			//alarm(2);
			printf("WAITING FOR SIGUSR1 SIGNAL\n");
			sigwait(&set, &sig);
			printf("RECEIVED SIGUSR1 SIGNAL\n");
			if(execvp(small_token_buffers[i].command_list[0], small_token_buffers[i].command_list) == -1){
				perror("Error: ");
				exit(1);
			}
			exit(-1);
		}else{
		}
	}

	signal(SIGALRM, alarm_handler);
	signal(SIGSTOP, alarm_handler);
	signal(SIGCONT, alarm_handler);

	alarm(2);
	signaller(pid_arr, n, SIGUSR1, n+1);
	signaller(pid_arr, n, SIGSTOP, n+1);
	signaller(pid_arr, n, SIGCONT, count);
	while(sum(child_procs, n) != n){
		if(alarmCall == 1){
			
			int status;
			signaller(pid_arr, n, SIGSTOP, count%n);
			waitpid(pid_arr[count%n], &status, WUNTRACED | WCONTINUED | WNOHANG);
			
			if(WIFEXITED(status) > 0){
				child_procs[count%n] = 1;
			}
			if(sum(child_procs, n) == n){
				break;
			}
			
			count++;
			while(child_procs[count%n] == 1){
				count++;
			}
			
			
			signaller(pid_arr, n, SIGCONT, count%n);
			alarmCall = 0;
			alarm(2);
		}
	}
	
	/*
	sleep(5);
	signaller(pid_arr, n, SIGUSR1);
	signaller(pid_arr, n, SIGSTOP);
	sleep(5);
	signaller(pid_arr, n, SIGCONT);
	sleep(3);
	signaller(pid_arr, n, SIGINT);
	for(int i = 0; i < n; i++){
		printf("Process: %d\n", pid_arr[i]);
	}
	*/
	free(pid_arr);
	return 0;
}