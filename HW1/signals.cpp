// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.hpp"

bool compareJob(const Job& job1, const Job& job2) {
    return job1.jid < job2.jid;
}

void insertSorted(vector<Job> &jobs, const Job &newJob) {
    auto it = lower_bound(jobs.begin(), jobs.end(), newJob, compareJob);
    jobs.insert(it, newJob);
}


void catch_ctrlc(int ctrlc){
	printf("smash: caught ctrl-C\n");
	if (fg_pid > 0 ){
		int res= kill(fg_pid,SIGKILL);
		if (!res){
			printf("smash: process %d was killed\n",fg_pid);
			fg_pid = -1;
			fg_jid = -1;
			fg_cmd = "";
		}
		else if (res == -1){
			 perror("smash error: kill failed");
		}
	}
}
//
//
void catch_ctrlz(int ctrlz){
	printf("smash: caught ctrl-Z\n");
	if (fg_pid > 0 ){
		int res= kill(fg_pid,SIGSTOP);
		if (!res){
			printf("smash: process %d was stopped\n",fg_pid);
			if (fg_jid > 0){
				Job job(fg_jid, fg_pid, 3, fg_cmd, time(nullptr));
				insertSorted(jobs,job);
			}
			else {
				if (!jobs.empty()) {
					Job job((jobs.back().jid) + 1, fg_pid, 3, fg_cmd, time(nullptr));
					jobs.push_back(job);
				}
				else{
					Job job(1, fg_pid, 3, fg_cmd, time(nullptr));
					jobs.push_back(job);
				}
			}
			fg_pid = -1;
			fg_jid = -1;
			fg_cmd = "";
		}
		else if (res == -1){
			 perror("smash error: kill failed");
		}
	}
}
//

