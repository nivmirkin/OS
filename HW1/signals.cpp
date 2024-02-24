// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.hpp"

//**************************************************************************************
// function name: compareJob
// Description: Function to compare Job objects based on their job IDs
// Parameters: const Job& job1, const Job& job2
// Returns: 0 - success,1 - failure
//**************************************************************************************

bool compareJob(const Job& job1, const Job& job2) {
    return job1.jid < job2.jid;
}

//**************************************************************************************
// function name: insertSorted
// Description: Function to insert a Job into a sorted vector of Jobs
// Parameters: vector<Job> &jobs, const Job &newJob
// Returns: void
//**************************************************************************************

void insertSorted(vector<Job> &jobs, const Job &newJob) {
    // Find the position to insert the newJob while maintaining sorted order
    auto it = lower_bound(jobs.begin(), jobs.end(), newJob, compareJob);
    // Insert the newJob into the vector
    jobs.insert(it, newJob);
}
//**************************************************************************************
// function name: catch_ctrlc
// Description: Signal handler for Control-C (SIGINT)
// Parameters: int ctrlc
// Returns: void
//**************************************************************************************
void catch_ctrlc(int ctrlc){
	cout << "smash: caught ctrl-C" <<endl;
	//printf("smash: caught ctrl-C\n");
    // Check if there is a foreground process to interrupt

	if (fg_pid > 0 ){
        // Attempt to terminate the foreground process with SIGKILL

		int res= kill(fg_pid,SIGKILL);
		if (!res){
			cout << "smash: process "<<fg_pid <<" was killed" << endl;
			//printf("smash: process %d was killed\n",fg_pid);
            // Reset foreground process information

			fg_pid = -1;
			fg_jid = -1;
			fg_cmd = "";
		}
		else if (res == -1){
			 perror("smash error: kill failed");
		}
	}
}
//**************************************************************************************
// function name: catch_ctrlz
// Description: Signal handler for Control-Z (SIGTSTP)
// Parameters: int ctrlz
// Returns: void
//**************************************************************************************
void catch_ctrlz(int ctrlz){
    // Print a message indicating the suspension by Control-Z
	cout << "smash: caught ctrl-Z" <<endl;
	//printf("smash: caught ctrl-Z\n");
    // Check if there is a foreground process to suspend
	if (fg_pid > 0 ){
    // Attempt to stop the foreground process with SIGSTOP

		int res= kill(fg_pid,SIGSTOP);
		if (!res){
			cout << "smash: process "<<fg_pid <<" was stopped" << endl;
			//printf("smash: process %d was stopped\n",fg_pid);
            // Add the stopped job to the list of jobs (sorted)

			if (fg_jid > 0){
				Job job(fg_jid, fg_pid, 3, fg_cmd, time(nullptr));
                // Insert the job into the sorted list of jobs

				insertSorted(jobs,job);
			}
			else {
                // If no job ID is provided, generate one based on the existing jobs

				if (!jobs.empty()) {
					Job job((jobs.back().jid) + 1, fg_pid, 3, fg_cmd, time(nullptr));
					jobs.push_back(job);
				}
				else{
				// If there are no existing jobs, create a job with ID 1

					Job job(1, fg_pid, 3, fg_cmd, time(nullptr));
					jobs.push_back(job);
				}
			}
			// Reset foreground process information

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

