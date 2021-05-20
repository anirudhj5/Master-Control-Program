#include<stdio.h>
#include <sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include <string.h>
#include "string_parser.h"

void script_print (pid_t* pid_ary, int size);

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

	pid_t *pid_arr = malloc(sizeof(pid_t)*n);
	for(int i = 0; i < n; i++){
		pid_arr[i] = fork();
		if(pid_arr[i] < 0){
			perror("Fork Error");
			exit(1);
		}else if(pid_arr[i] == 0){
			if(execvp(small_token_buffers[i].command_list[0], small_token_buffers[i].command_list) == -1){
				perror("Error: ");
				exit(1);
			}
			exit(-1);
		}else{
			wait(NULL);
		}
	}

	for(int j = 0; j < n; j++){
		free_command_line(&small_token_buffers[j]);
	}


	script_print(pid_arr, n);
	free(pid_arr);
	fclose(fp);
	return 0;
}


void script_print (pid_t* pid_ary, int size)
{
	FILE* fout;
	fout = fopen ("top_script.sh", "w");
	fprintf(fout, "#!/bin/bash\ntop");
	for (int i = 0; i < size; i++)
	{
		fprintf(fout, " -p %d", (int)(pid_ary[i]));
	}
	fprintf(fout, "\n");
	fclose (fout);
	
	char* top_arg[] = {"gnome-terminal", "--", "bash", "top_script.sh", NULL};
	pid_t top_pid;

	top_pid = fork();
	{
		if (top_pid == 0)
		{
			if(execvp(top_arg[0], top_arg) == -1)
			{
				perror ("top command: ");
			}
			exit(0);
		}
	}
}


