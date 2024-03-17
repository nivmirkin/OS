
#include "Bank.hpp"
#include "ATM.hpp"
#include <map>

#define LOG "log.txt"


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% function of bank%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
bank::bank(){
	
}
//%%%%%%%%%
bank::bank_commissions_thread(){
	whlie(){
		int Commission_Percent = rand() % 5 + 1;
		updating_BankBalance(Commission_Percent);
		sleep(3);
	}
}
//%%%%%%%%
bank::updating_BankBalance(int Commission_Percent ){
	 for (auto it = accounts.begin(); it != accounts.end(); ++it) {
		 if(it->get_amount > 0){
			 int Commission_from_acc =(int) round((double)( Commission_Percent * it->get_amount) / 100) ;
			 it.balance -= Commission_from_acc;
			 this.BankBalance += Commission_from_acc;
		 }
		 
	 }
}
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& function of accounts &&&&&&&&&&&&&&&&&&
Account::Account(int id, string pwd, int amt) : ID(id), password(pwd), amount(amt), read_cnt(0) {
	pthread_mutex_init(&write_lock, NULL);
	pthread_mutex_init(&read_lock, NULL);
}
Account::~Account() {
}

void Account::lock_write(void) {
	pthread_mutex_lock(&write_lock);
}
void Account::unlock_write(void) {
	pthread_mutex_unlock(&write_lock);
}

void Account::lock_read(void) {
	pthread_mutex_lock(&read_lock);
	read_cnt++;
	if (read_cnt == 1) {
		pthread_mutex_lock(&write_lock);
	}
	pthread_mutex_unlock(&read_lock);
}
void Account::unlock_read(void) {
	pthread_mutex_lock(&read_lock);
	read_cnt--;
	if (read_cnt == 0) {
		pthread_mutex_unlock(&write_lock);
	}
	pthread_mutex_unlock(&read_lock);
}

bool Account::comparePassword(string pwd) {
	return password == pwd;
}

int Account::updateAmount(int addedAmount) {
	lock_write();
	int res;
	res = amount + addedAmount;
	if (res >= 0) {
		amount = res;
	}
	else {
		res = (-1) * amount;
	}
	unlock_write();
	return res;

}
int Account::getAmount() {
	int res;
	lock_read();
	res = amount;
	unlock_read();
	return res;
}

void  Account::lockAccount() { accountMutex.lock(); }
void  Account::unlockAccount() { accountMutex.unlock(); }
};
//######################################### global ########################################
pthread_mutex_t log_lock = PTHREAD_MUTEX_INITIALIZER;
map<int, Account> accounts;
//***************************************** main *******************************************
int main (int argc, char *argv[]) {
	int N = argc -1 ;
	
	//check if there is no parameters given print error	
	if(argc == 1){
		cerr << "Bank error: illegal arguments" << endl;
	}
	if(open(LOG,O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)){
		perror("Bank error: open failed");
		exit(1);
	}
	//creat N threads 
	pthread_t* atm_threads = new pthread_t[N];
	//creating a vec to store all the ATMs
	vector <ATM> atm_vec;
	//creat bank thread for the commition and for the balance printing 
	pthread_t bank_commissions_thread;
	pthread_t bank_print_BankBalance;
	
	//creating a mutex 
	if (pthread_mutex_init(&log_lock, nullptr) != 0) {
		perror("Bank error: pthread_mutex_init failed");
		exit(1);
	}
	//*********************************creating threads***********************************
	//creating the commition thread
	if (pthread_create(&bank_commissions_thread, nullptr, Bank::bank_commissions_thread_init, nullptr)) {
		 perror("Bank error: pthread_create failed");
		 close(logFile);
		 atm_vec.clear();
		 delete[] atm_threads;
		 pthread_mutex_destroy(&log_lock);
		 exit(1);
	 }
	//creating the printing the bank balance thread
	if (pthread_create(&bank_print_BankBalance, nullptr, Bank::bank_print_BankBalance, nullptr)) {
		 perror("Bank error: pthread_create failed");
		 close(logFile);
		 atm_vec.clear();
		 delete[] atm_threads;
		 pthread_mutex_destroy(&log_lock);
		 exit(1);
	 }
	
	//putint the files in to the atm class with the ID and creating N threads for ATMs
	for (int i =1 ; i=< N ; i++){
			ATM atm(i);
			if( atm.loadFile(argv[i]))
			{
				perror("Bank error: open failed");
				exit(1);
			}
			atm_vec.push_back(atm);
	
			// Create a thread for each ATM
			if (pthread_create(atm_threads + i - 1, nullptr, atm_thread_init, &atm_vec[i - 1])) {
				perror("Bank error: pthread_create failed");
				atm_vec.clear();
				delete[] atm_threads;
				pthread_mutex_destroy(&log_lock);
				close(logFile);
				exit(1);
			}
	}
	//***********************************************************************
	// joining all the threads 
	for (int i =1 ; i=< N ; i++){
		if(pthread_join(atm_threads[i], nullptr) != 0 ){
			perror("Bank error: pthread_join failed");
		}
	}
	if (pthread_join(bank_commissions_thread, nullptr) != 0) {
		perror("Bank error: pthread_join failed");
	
	}
	
	// Wait for bank print thread to finish
	if (pthread_join(bank_print_BankBalance, nullptr) != 0) {
		perror("Bank error: pthread_join failed");
	
	}
	//*********************ending deleting/freeing and closeing everything that had been used********** 
	atm_vec.clear();
	delete[] atm_threads;
	pthread_mutex_destroy(&log_lock);
	
	if(close(LOG) == -1){
			perror("Bank error: close failed");
			exit(1);
	}
	
	return SUCCESS
        
}



