//		commands.c
//********************************************
#include "commands.hpp"
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
char oldpwd[MAX_LINE_SIZE];

Job::Job(int jid, int pid, int status, const string& command, time_t starttime)
    : jid(jid), pid(pid), status(status), command(command), starttime(starttime) {}

void Job::print(bool printTime /* = true */) {
    double secondsElapsed = difftime(time(nullptr), this->starttime);

    string stopped = "";
    if (this->status == 3) stopped = " (stopped)";

    string timeStr = "";

    if (printTime) {
        std::ostringstream ss;
        ss << " " << std::fixed << std::setprecision(0) << secondsElapsed << " secs" << stopped;
        timeStr = ss.str();
    }

    cout << "[" << this->jid << "] " << this->command << " : " << this->pid << timeStr << endl;
}

//********************************************
// function name: CleanupJobs
// Description: cleaning up finished jobs function
// Parameters: vector<Job>& jobs
// Returns: void
//**************************************************************************************
void CleanupJobs(vector<Job>& jobs) {
    if(jobs.empty()){
    	return ;
    }
    
    for (auto it = jobs.begin(); it != jobs.end();) {
    	
      int res=waitpid(it->pid, nullptr, WNOHANG);
      if (res==0){
    	  it++;
      }
      else if((res==it->pid)||(errno==ECHILD)){
		  it = jobs.erase(it);
      }
      else{
    	  perror("smash error: waitpid failed");
      }
    }
}
//////////////////////////

//********************************************
// function name: ExeCmd
// Description:  Function to execute built-in commands and external commands
// Parameters: vector<Job>& jobs, char* lineSize, char* cmdString
// Returns: (int) 0-command execution was successful 1-command execution encountered an error or failed.
//**************************************************************************************
int ExeCmd(vector<Job>& jobs, char* lineSize, char* cmdString)
{

	
	char* cmd;
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	string delimiters = " \t\n";
	int i = 0, num_arg = 0;
	bool illegal_cmd = false; // illegal command
    	cmd = strtok(lineSize, delimiters.c_str());
	if (cmd == NULL)
		return 0; 
   	args[0] = cmd;

	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters.c_str()); 
		if (args[i] != NULL) 
			num_arg++; 
 
	}
	CleanupJobs(jobs);
/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
	if (!strcmp(cmd, "cd") ) 
	{
		if (num_arg > 1){
			cerr << "smash error: cd: too many arguments" << endl; 
			return 1;
		}
		if (getcwd(pwd, sizeof(pwd)) == NULL){
			perror("smash error: getcwd failed");
		}
		else{
			if (!strcmp(args[1], "-")){
				if (oldpwd == NULL || !strcmp(oldpwd,"\0")){
					cerr << "smash error: cd: OLDPWD not set" <<endl;
					return 1;
				}
				char tmp[MAX_LINE_SIZE];
				strcpy(tmp,oldpwd);
				if (chdir(tmp) != 0){
					perror("smash error: chdir failed");
				}
				strcpy(oldpwd,pwd);
			}
			else{
				if (chdir(args[1]) != 0){
					perror("smash error: chdir failed");
				}
				strcpy(oldpwd,pwd);
			}
		}
	} 
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		if (getcwd(pwd, sizeof(pwd)) == NULL){
				perror("smash error: getcwd failed");
		}
		else{
			cout << pwd <<endl;
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "jobs")) 
	{	
		
		for (auto it = jobs.begin(); it != jobs.end(); ++it) {
		// Dereference the iterator to access the Job object
			Job& job = *it;
		    // Do something with the job
		    job.print();
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
		pid_t pid = getpid();
		if (pid == -1) {
			perror("smash error: showpid failed");
			return 1;
		}
		cout << "smash pid is " << pid << endl;
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{
		bool found = false;
		int jobid = -1;
		if (num_arg > 1){
			cerr << "smash error: fg: invalid arguments" <<endl;
			return 1;
		}
		else if (num_arg == 1){
			string arg1 = args[1];
			jobid = atoi(arg1.c_str());
			if (!jobid){
				cerr << "smash error: fg: invalid arguments" << endl;
				return 1;
			}
		}
		if ((jobid==-1) && jobs.empty()){
			cerr << "smash error: fg: jobs list is empty" << endl;
			return 1;
		}
		for (auto it = jobs.end()-1; it != jobs.begin()-1; it--) {
			// Dereference the iterator to access the Job object

			Job& job = *it;
			// Do something with the job
			if (jobid == job.jid || jobid == -1){
				found = true;
				fg_cmd = job.command;
				fg_pid = job.pid;
				fg_jid = job.jid;
				job.print(false);
				jobs.erase(it);
				int res = kill(fg_pid,SIGCONT);
				if (!res){
					waitpid(fg_pid,nullptr,0);
				}
				else if (res == -1){
					perror("smash error: kill failed");
				}
				fg_cmd = "";
				fg_pid = -1;
				fg_jid = -1;
				return 0;
			}
		}

		if (jobs.empty() || !found ){
			cerr << "smash error: fg: job-id "<< jobid<<" does not exist" << endl;
			return 1;
		}

	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
		bool found = false;
		int jobid = 0;
		if (num_arg > 1){
			cerr << "smash error: bg: invalid arguments" << endl;
			return 1;
		}
		else if (num_arg == 1){
			string arg1 = args[1];
			jobid = atoi(arg1.c_str());
		}
		for (auto it = jobs.end()-1; it != jobs.begin()-1; it--) {
			// Dereference the iterator to access the Job object
			Job& job = *it;
			// Do something with the job
			if (jobid == job.jid || !jobid){
				if (jobid == job.jid && job.status == 2){
					cerr << "smash error: bg: job-id "<< jobid <<" is already running in the background" <<endl;
					return 0;
				}
				if (job.status == 3){
					found = true;
					job.print(false);
					int res = kill(job.pid,SIGCONT);
					if (res == -1){
						perror("smash error: kill failed");
					}
					job.status = 2;
					return 0;
				}
			}
		}
		if (!jobid && !found){
			cerr << "smash error: bg: there are no stopped jobs to resume" <<endl;
			return 1;
		}

		if (jobs.empty() || !found ){
			cerr << "smash error: bg: job-id "<< jobid <<" does not exist\n" << endl;
			return 1;
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
		bool term = false;
		if (num_arg > 0){
			string arg1 = args[1];
			if (arg1 == "kill") term = true;
		}

		if (term){
			for (auto it = jobs.begin(); it != jobs.end(); ++it) {
				string cmd = it->command;
				cout << "["<< it->pid << "] "<< it->command << " - Sending SIGTERM..." << flush;
				int res = kill(it->pid,SIGTERM);
				if (res == -1){
					perror("smash error: kill failed");
				}
				sleep(5);
				if (waitpid(it->pid,nullptr,WNOHANG)){
					cout << "Done." << endl;
				}
				else{
					cout << "(5 sec passed) Sending SIGKILL..." << flush;
					res = kill(it->pid,SIGKILL);
					if (res == -1){
						perror("smash error: kill failed");
					}
					cout << "Done." <<endl;
				}
			}
		}
		jobs.clear();
		exit(0);
	} 
	/*************************************************/
	else if (!strcmp(cmd, "kill"))
	{
		string arg1 = args[1];
		string arg2 = args[2];
		if (num_arg > 2 || args[1][0] != '-' || arg1.size()!=2){
			cerr << "smash error: kill: invalid arguments" << endl;
			return 1;
		}
		int signalNumber = atoi(arg1.substr(1).c_str());
		int jobid = atoi(arg2.c_str());
		bool found = false;
		for (auto it = jobs.begin(); it != jobs.end(); ++it) {
			// Dereference the iterator to access the Job object
			Job& job = *it;
			// Do something with the job
			if (jobid == job.jid){
				found= true;
				int res = kill(job.pid,signalNumber);
				if (res == -1){
					perror("smash error: kill failed");
				}
				else if(!res){
					cout << "signal number "<< signalNumber << " was sent to pid " << job.pid <<endl;
					jobs.erase(it);
				}
				return 0;
			}

		}
		if (!found){
			cerr << "smash error: kill:job-id "<< jobid <<" does not exist" <<endl;
			return 1;
		}

	} 
	/*************************************************/	
	else if (!strcmp(cmd, "diff"))
	{	
		bool equal = true ;
		if (num_arg != 2 ){
				cerr << "smash error: diff: invalid arguments" << endl;
				return 1;
		}
		
		int file1 = open (args[1] , O_RDONLY);
		int file2 = open (args[2] , O_RDONLY);
		if (file1 == -1 || file2 == -1) {
		    perror("smash error: open failed");
		        return 1;
		}
		char buffer1[BUFFER_SIZE];
		char buffer2[BUFFER_SIZE];
		ssize_t bytesRead1, bytesRead2;
		off_t i = 0;
		while(equal){
			bytesRead1 = read(file1,buffer1, BUFFER_SIZE);
			bytesRead2 = read(file2,buffer2, BUFFER_SIZE);
			
			if (bytesRead1 == -1 || bytesRead2 == -1) {
				        perror("smash error: read failed");
				        return 1;
			}
			
			if (bytesRead1 == 0 && bytesRead2 == 0) {
			            // Both files reached the end simultaneously, and they are identical
			            
			            break;
			}
			i += bytesRead1;
			
	        if (bytesRead1 != bytesRead2 || memcmp(buffer1, buffer2, bytesRead1) != 0){//bytesRead1 == 0 || bytesRead2 == 0 || buffer1 != buffer2) {
	        	equal = false ;
	            break;
	        }
		}
		close(file1);
		close(file2);
		if(equal){
			cout << "0" <<endl;
		}else{
			cout << "1" <<endl;
		}
		
	}

		
	 
	/*************************************************/	
	else // external command
	{
 		ExeExternal(args, cmdString);
	 	return 0;
	}
	if (illegal_cmd == true)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}
    return 0;
    

}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG], char* cmdString)
{
	
	bool run_bg = false;
	int i = 0;
	while (args[i + 1] != NULL) {
	    i++;
	}
	if (!strcmp(args[i], "&")){
        run_bg = true;
        args[i]=nullptr;
    }
	int pID;
    	switch(pID = fork()) 
    	{
    		case -1: 
					// Add your code here (error)
		        perror("smash error: fork failed");
        	case 0 :
                	// Child Process
        		    if (setpgrp() == -1) {
        		    	perror("smash error: setpgrp failed");
        		        exit(EXIT_FAILURE);
        		    }
			        // Add your code here (execute an external command)
					
               	    if (execvp(args[0], args) == -1) {
               	        perror("smash error: execvp failed");
               	        exit(1);
               	    }
			
			default:
		
				    if (run_bg == false){
				    	fg_pid = pID;
				    	fg_cmd = cmdString;
				    	if (waitpid(fg_pid, nullptr, WUNTRACED) == -1) {
				    		if ((errno != ECHILD) && (errno != EINTR)) {
				    			perror("smash error: waitpid failed");
				    		}
				    	}
				    	fg_pid = -1;
				    	fg_cmd = "";
				    }
				    else{
				    	if (!jobs.empty()) {
				    	    Job job((jobs.back().jid) + 1, pID, 2, cmdString, time(nullptr));
				    	    jobs.push_back(job);
				    	}
				    	else{
				    		 Job job(1, pID, 2, cmdString, time(nullptr));
				    		 jobs.push_back(job);
				    	}
				    }
				  

					
					
	}
}

//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, void* jobs)
{

//	char* Command;
//	char* delimiters = " \t\n";
//	char *args[MAX_ARG];
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';
		// Add your code here (execute a in the background)
					
		/* 
		your code
		*/
		
	}
	return -1;
}

