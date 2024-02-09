#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h> 
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iomanip>
#include <string>
#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>
#define MAX_LINE_SIZE 80
#define MAX_ARG 20
#define BLOCK_SIZE 1000
using namespace std;
class Job {
public:
    int jid;          // Job ID
    int pid;          // Process ID
    int status;       // Status of the job
    string command;  // Command associated with the job
    time_t starttime; // Start time of the job

    // Constructor
    Job(int jid, int pid, int status, const string& command, time_t starttime);

    void print(bool printTime=true);
};
extern int fg_jid;
extern int fg_pid;
extern string fg_cmd;
extern vector<Job> jobs;
//typedef enum { FALSE , TRUE } bool;
int BgCmd(char* lineSize, void* jobs);
int ExeCmd(vector<Job>& jobs, char* lineSize, char* cmdString);
void ExeExternal(char *args[MAX_ARG], char* cmdString);

#endif
