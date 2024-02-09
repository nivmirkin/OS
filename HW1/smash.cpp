/*	smash.c
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "commands.hpp"
#include "signals.hpp"
#include <vector>
#define MAX_LINE_SIZE 80
#define MAXARGS 20

char* L_Fg_Cmd;
vector <Job> jobs; //This represents the list of jobs. Please change to a preferred type (e.g array of char*)
char lineSize[MAX_LINE_SIZE]; 
int fg_jid=-1; // TODO check if needed
int fg_pid=-1;
std::string fg_cmd="";

//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{
    char cmdString[MAX_LINE_SIZE]; 	   
    struct sigaction act_c, act_z;
	
	//signal declaretions
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	 /* add your code here */
    act_c.sa_handler = &catch_ctrlc;
    sigemptyset(&act_c.sa_mask);
    act_c.sa_flags = 0;
    sigaction(SIGINT, &act_c, nullptr);

	/************************************/
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	//set your signal handlers here
	/* add your code here */

	/************************************/
    act_z.sa_handler = &catch_ctrlz;
    sigemptyset(&act_z.sa_mask);
    act_z.sa_flags = 0;
    sigaction(SIGTSTP, &act_z, nullptr);
	/************************************/
	// Init globals 
 // delete
 	Job job1(1, 101, 2, "cmd1",time(nullptr));
 	jobs.push_back(job1);
   	Job job2(2, 103, 3, "cmd2",time(nullptr));
   	jobs.push_back(job2);
	
	L_Fg_Cmd =(char*)malloc(sizeof(char)*(MAX_LINE_SIZE+1));
	if (L_Fg_Cmd == NULL) 
			exit (-1); 
	L_Fg_Cmd[0] = '\0';
	
    	while (1)
    	{
	 	printf("smash > ");
		fgets(lineSize, MAX_LINE_SIZE, stdin);
		strcpy(cmdString, lineSize);    	
		cmdString[strlen(lineSize)-1]='\0';
					// background command	
	 	//if(!BgCmd(lineSize, jobs)) continue; //TODO bring back
					// built in commands
		ExeCmd(jobs, lineSize, cmdString);
		
		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
	}
    return 0;
}

