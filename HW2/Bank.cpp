
#include "Bank.hpp"
#include "ATM.hpp"
#include <map>

#define LOG "log.txt"


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% function of bank%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Bank::Bank() : BankBalance(0) {}
//%%%%%%%%%
static void* Bank::bank_commissions(void* pbank){
	Bank* bank = static_cast<Bank*>(pbank);
	int acc_cmsn, cmsn_perc;
	while(true){
		cmsn_perc = rand() % 5 + 1;
		bank->lock_read();
		for (auto it = bank->accounts.begin(); it != bank->accounts.end(); ++it) {
			acc_cmsn = it->second->cmsnWithdraw(cmsn_perc);
			bank->BankBalance += acc_cmsn;
			cout << "Bank: commissions of " << cmsn_perc << " % were charged, the bank gained " << acc_cmsn << " $ from account " << it->first << endl;
		}
		bank->unlock_read();
		sleep(3);
	}

int Bank::bank(){
	pthread_mutex_init(&bankBalanceLock, nullptr);
}

int Bank::bank_print_Balance(){//print stat to screen
	while(true){
		bank.print_stat();
		sleep(0.5);
	}
	
}
void Bank::print_stat(){
	pthread_mutex_lock(&bankBalanceLock); 
	int current_bank_balance = BankBalance;

    printf("\033[2J"); // clear screen
    printf("\033[1;1H"); //move cursor
    cout << "Current Bank Status" << endl ;
    string pW ;
	for (auto const& pair : accounts) {
	   int id = pair.first;
	   int balance = pair.second->getAmount(&pW );
	   cout << "Account " << id << ": Balance – " << balance <<" $, Account Password – " << pW << endl;
	}
	 pthread_mutex_unlock(&bankBalanceLock);
	 cout << "The Bank has " << currentBankBalance << " $" << endl;
}
int Bank::bank_commissions_thread(){
	//whlie(true){
	//	int Commission_Percent = rand() % 5 + 1;
	//	updating_Banknce(Commission_Percent);
	//	sleep(3);
	//}
	return 0;
}
//%%%%%%%%
int Bank::updating_Banknce(int Commission_Percent ){
	 for (auto it = accounts.begin(); it != accounts.end(); ++it) {
		 if(it->second->getAmount()> 0) {
			// int Commission_from_acc =(int) round((double)( Commission_Percent * it->get_amount) / 100) ;    //TODO replace with function within account
			// it.nce -= Commission_from_acc;
			// this.Banknce += Commission_from_acc;
		 }
		 
	 }
	 return 0;

}

int Bank::addAcc(int id, string pswd, int amount) {
	int res = SUCCESS;
	lock_write();
	if (accounts.find(id) != accounts.end()) {
		res = ACC_EXST;
	}
	else {
		Account acnt(id, pswd, amount);
		accounts.emplace(id, &acnt);
	}
	unlock_write();
	return res;
}

int Bank::deposit(int id, string pswd, int amount) {
	int res;
	lock_read();
	auto it = accounts.find(id);
	if (it == accounts.end()) {
		res = ACC_NOT_EXST;
	}
	else {
		if (!it->second->comparePassword(pswd)) {
			res = WROND_PSWD;
		}
		else{
			res = it->second->updateAmount(amount);
		}
	}
	unlock_read();
	return res;
}

int Bank::withdraw(int id, string pswd, int amount) {
	int res;
	lock_read();
	auto it = accounts.find(id);
	if (it == accounts.end()) {
		res = ACC_NOT_EXST;
	}
	else {
		if (!it->second->comparePassword(pswd)) {
			res = WROND_PSWD;
		}
		else {
			res = it->second->updateAmount(-amount);
			if (res < 0) {
				res = INSFCNT_FUNDS;
			}
		}
	}
	unlock_read();
	return res;
}


int Bank::checknce(int id, string pswd) {
	int res;
	lock_read();
	auto it = accounts.find(id);
	if (it == accounts.end()) {
		res = ACC_NOT_EXST;
	}
	else {
		if (!it->second->comparePassword(pswd)) {
			res = WROND_PSWD;
		}
		else {
			res = it->second->getAmount();
		}
	}
	unlock_read();
	return res;
}


int Bank::removeAcc(int id,string pswd) {
	int res;
	lock_write();
	auto it = accounts.find(id);
	if (it == accounts.end()) {
		res = ACC_NOT_EXST;
	}
	else {
		if (!it->second->comparePassword(pswd)) {
			res = WROND_PSWD;
		}
		else {
			res = it->second->getAmount();
			accounts.erase(it);
		}
	}
	unlock_write();
	return res;
}

int  Bank::transer(int from_id, string pswd, int to_id, int amount ,int* from_nce) {
	int res;
	lock_read();
	auto it = accounts.find(from_id);
	auto to_it = accounts.find(to_id);
	if (to_it == accounts.end()) {
		res = TO_ACC_NOT_EXST;
	}
	else if (it == accounts.end()) {
		res = ACC_NOT_EXST;
	}
	else {
		if (!it->second->comparePassword(pswd)) {
			res = WROND_PSWD;
		}
		else {
			res = it->second->updateAmount(-amount);
			if (res < 0) {
				res = INSFCNT_FUNDS;
			}
			else {
				*from_nce = res;
				res = to_it->second->updateAmount(amount);
			}
		}
	}
	unlock_read();
	return res;
}


void Bank::lock_write(void) {
	pthread_mutex_lock(&write_lock);
}
void Bank::unlock_write(void) {
	pthread_mutex_unlock(&write_lock);
}

void Bank::lock_read(void) {
	pthread_mutex_lock(&read_lock);
	read_cnt++;
	if (read_cnt == 1) {
		pthread_mutex_lock(&write_lock);
	}
	pthread_mutex_unlock(&read_lock);
}
void Bank::unlock_read(void) {
	pthread_mutex_lock(&read_lock);
	read_cnt--;
	if (read_cnt == 0) {
		pthread_mutex_unlock(&write_lock);
	}
	pthread_mutex_unlock(&read_lock);
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
int Account::cmsnWithdraw(int per) {
	lock_write();
	int res;

	res = (int)(round(((double)amount*per)/100.0));
	amount -= res;
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
int Account::getAmount(string * pW) {
	int res;
	lock_read();
	res = amount;
	*pW = password ;
	unlock_read();
	return res;
}
int Account::getAmount(string * pW) {
	int res;
	lock_read();
	res = amount;
	*pW = password ;
	unlock_read();
	return res;
}

//######################################### global ########################################
pthread_mutex_t log_lock = PTHREAD_MUTEX_INITIALIZER;
Bank bank;
//***************************************** main *******************************************
int main (int argc, char *argv[]) {
	
	int Nthreads = argc -1 ;
	//check if there is no parameters given print error	
	if(argc == 1){
		cerr << "Bank error: illegal arguments" << endl;
	}
	int logFile = open(LOG, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if(logFile){
		perror("Bank error: open failed");
		exit(1);
	}
	//creat N threads 
	pthread_t* atm_threads = new pthread_t[Nthreads];
	//creating a vec to store all the ATMs
	vector <ATM> atm_vec;
	//creat bank thread for the commition and for the nce printing 
	pthread_t bank_commissions_thread;
	pthread_t bank_print_Balance;
	
	//creating a mutex 
	if (pthread_mutex_init(&log_lock, nullptr) != 0) {
		perror("Bank error: pthread_mutex_init failed");
		exit(1);
	}
	//*********************************creating threads***********************************
	//creating the commition thread
	if (pthread_create(&bank_commissions_thread, nullptr, Bank::bank_commissions, ((void*)&bank))) {
		 perror("Bank error: pthread_create failed");
		 close(logFile);
		 atm_vec.clear();
		 delete[] atm_threads;
		 pthread_mutex_destroy(&log_lock);
		 exit(1);
	 }
	//creating the printing the bank nce thread
	if (pthread_create(&bank_print_Balance, nullptr, Bank::bank_print_Balance, nullptr)) {
		 perror("Bank error: pthread_create failed");
		 close(LOG);
		 atm_vec.clear();
		 delete[] atm_threads;
		 pthread_mutex_destroy(&log_lock);
		 exit(1);
	 }
	
	//putint the files in to the atm class with the ID and creating N threads for ATMs
	for (int i =1 ; i <= Nthreads; i++){
			ATM atm(i);
			if( atm.loadFile(argv[i]))
			{
				perror("Bank error: open failed");
				exit(1);
			}
			atm_vec.push_back(atm);
	
			// Create a thread for each ATM
			if (pthread_create(atm_threads + i - 1, nullptr, ATM::ATMrun, ((void*)&atm))) {
				perror("Bank error: pthread_create failed");
				atm_vec.clear();
				delete[] atm_threads;
				pthread_mutex_destroy(&log_lock);
				close(LOG);
				exit(1);
			}
	}
	//***********************************************************************
	// joining all the threads 
	for (int i =1 ; i <= Nthreads; i++){
		if(pthread_join(atm_threads[i], nullptr) != 0 ){
			perror("Bank error: pthread_join failed");
		}
	}
	if (pthread_join(bank_commissions_thread, nullptr) != 0) {
		perror("Bank error: pthread_join failed");
	
	}
	
	// Wait for bank print thread to finish
	if (pthread_join(bank_print_Balance, nullptr) != 0) {
		perror("Bank error: pthread_join failed");
	
	}
	//*********************ending deleting/freeing and closeing everything that had been used********** 
	atm_vec.clear();
	delete[] atm_threads;
	pthread_mutex_destroy(&log_lock);
	
	if(close(logFile) == -1){
			perror("Bank error: close failed");
			exit(1);
	}
	
	return SUCCESS;
        
}



