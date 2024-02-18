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

bool compareJob(const Job& job1, const Job& job2);
void insertSorted(vector<Job> &jobs, const Job &newJob);
void catch_ctrlc(int ctrlc);
void catch_ctrlz(int ctrlz);
#endif

