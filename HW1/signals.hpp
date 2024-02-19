#ifndef _SIGS_H
#define _SIGS_H
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <string>
#include <vector>
#include <algorithm>
#include "commands.hpp"
using namespace std;

extern int fg_jid;
extern int fg_pid;
extern string fg_cmd;
extern vector<Job> jobs;

bool compareJob(const Job& job1, const Job& job2);// Function to compare Job objects based on their job IDs
void insertSorted(vector<Job> &jobs, const Job &newJob);// Function to insert a Job into a sorted vector of Jobs
void catch_ctrlc(int ctrlc);// Signal handler for Control-C (SIGINT)
void catch_ctrlz(int ctrlz);// Signal handler for Control-Z (SIGTSTP)
#endif

