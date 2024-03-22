
#include "Bank.hpp"
#include "ATM.hpp"
#include <map>

#define LOG "log.txt"


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% function of bank%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Bank::Bank() : BankBalance(0) {
	pthread_mutex_init(&bankBalanceLock, nullptr);
	pthread_mutex_init(&log_lock, nullptr);
	pthread_mutex_init(&write_lock, nullptr);
	pthread_mutex_init(&read_lock, nullptr);

}
//%%%%%%%%%
void* Bank::bank_commissions(void* pbank) {
	Bank* bank = static_cast<Bank*>(pbank);
	int acc_cmsn, cmsn_perc;
	while (true) {
		sleep(3);
		ostringstream buff;
		cmsn_perc = rand() % 5 + 1;
		bank->lock_read();
		for (auto it = bank->accounts.begin(); it != bank->accounts.end(); ++it) {
			acc_cmsn = it->second->cmsnWithdraw(cmsn_perc);
			bank->BankBalance += acc_cmsn;
			buff << "Bank: commissions of " << cmsn_perc << " % were charged, the bank gained " << acc_cmsn << " $ from account " << it->first << endl;
		}
		bank->writeLog(&buff);
		bank->unlock_read();
	}
}

void* Bank::bank_print_Balance(void* pbank){//print stat to screen
	Bank* bank = static_cast<Bank*>(pbank);
	while(true){
		bank->print_stat();
        usleep(500000);
	}
}
void Bank::print_stat(void){
	pthread_mutex_lock(&bankBalanceLock); 
	int current_bank_balance = BankBalance;
	lock_read();
    printf("\033[2J"); // clear screen
    printf("\033[1;1H"); //move cursor
    cout << "Current Bank Status" << endl ;
    string pW ;
	for (auto const& pair : accounts) {
	   int id = pair.first;
	   int balance = pair.second->getAmount(&pW );
	   cout << "Account " << id << ": Balance – " << balance <<" $, Account Password – " << pW << endl;
	}
	unlock_read();
	 pthread_mutex_unlock(&bankBalanceLock);
	 cout << "The Bank has " << current_bank_balance << " $" << endl;
}

int Bank::addAcc(int id, string pswd, int amount) {
	int res = SUCCESS;
	Account* acnt;
	lock_write();
	sleep(1);
	if (accounts.find(id) != accounts.end()) {
		res = ACC_EXST;
	}
	else {
		acnt = new Account(id, pswd, amount);
		accounts.emplace(id, acnt);
	}
	unlock_write();
	return res;
}

int Bank::deposit(int id, string pswd, int amount) {
	int res;
	lock_read();
	sleep(1);
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
	sleep(1);
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


int Bank::checkBalance(int id, string pswd) {
	int res;
	lock_read();
	sleep(1);
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
	sleep(1);
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
	sleep(1);
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

void Bank::writeLog(ostringstream* buff){
	pthread_mutex_lock(&log_lock);
	logFile << (*buff).str() << flush;
	pthread_mutex_unlock(&log_lock);
}
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& function of accounts &&&&&&&&&&&&&&&&&&
Account::Account(int id, string pwd, int amt) : ID(id), password(pwd), amount(amt), read_cnt(0) {
	pthread_mutex_init(&acc_write_lock, NULL);
	pthread_mutex_init(&acc_read_lock, NULL);
}
Account::~Account() {
}

void Account::lock_write(void) {
	pthread_mutex_lock(&acc_write_lock);
}
void Account::unlock_write(void) {
	pthread_mutex_unlock(&acc_write_lock);

}

void Account::lock_read(void) {
	pthread_mutex_lock(&(this->acc_read_lock));

	this->read_cnt++;
	if (this->read_cnt == 1) {
		pthread_mutex_lock(&acc_write_lock);
	}
	pthread_mutex_unlock(&acc_read_lock);
}
void Account::unlock_read(void) {
	pthread_mutex_lock(&acc_read_lock);
	read_cnt--;
	if (read_cnt == 0) {
		pthread_mutex_unlock(&acc_write_lock);
	}
	pthread_mutex_unlock(&acc_read_lock);
}

bool Account::comparePassword(string pwd) {
	return this->password == pwd;
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
	this->lock_read();
	res = amount;
	*pW = password ;
	this->unlock_read();
	return res;
}


//######################################### global ########################################
ofstream logFile(LOG);
Bank bank;
//***************************************** main *******************************************
int main (int argc, char *argv[]) {
	
	int Nthreads = argc -1 ;
	//check if there is no parameters given print error	
	if(argc == 1){
		cerr << "Bank error: illegal arguments" << endl;
	}
	if(!logFile.is_open()){
		perror("Bank error: open failed");
		exit(1);
	}
	//creat N threads 
	pthread_t* atm_threads = new pthread_t[Nthreads];
	//creating a vec to store all the ATMs
	vector <ATM*> atm_vec;
	//creat bank thread for the commition and for the nce printing 
	pthread_t bank_commissions_thread;
	pthread_t bank_print_Balance;
	
	//creating a mutex //TODO add hanle to all inits
	/*if (pthread_mutex_init(&bank-log_lock, NULL) != 0) {
		perror("Bank error: pthread_mutex_init failed");
		exit(1);
	}*/
	//*********************************creating threads***********************************
	//creating the commition thread
	if (pthread_create(&bank_commissions_thread, NULL, Bank::bank_commissions, ((void*)&bank))) {
		 perror("Bank error: pthread_create failed");
		 logFile.close();
		 atm_vec.clear();
		 delete[] atm_threads;
		// pthread_mutex_destroy(&(bank.log_lock));
		 exit(1);
	 }
	//creating the printing the bank nce thread
	if (pthread_create(&bank_print_Balance, NULL, Bank::bank_print_Balance, ((void*)&bank))) {
		 perror("Bank error: pthread_create failed");
		 logFile.close();
		 atm_vec.clear();
		 delete[] atm_threads;
		 //pthread_mutex_destroy(&(bank.log_lock));
		 exit(1);
	 }
	
	//putint the files in to the atm class with the ID and creating N threads for ATMs
	for (int i =1 ; i <= Nthreads; i++){
		ATM* atm;
		atm = new ATM(i, argv[i]);
		atm_vec.push_back(atm);
		
		// Create a thread for each ATM
		if (pthread_create(atm_threads + i - 1, NULL, ATM::ATMrun, ((void*)atm))) {
			perror("Bank error: pthread_create failed");
			atm_vec.clear();
			delete[] atm_threads;
			//pthread_mutex_destroy(&(bank.log_lock));
			logFile.close();
			exit(1);
		}
	}
	//***********************************************************************
	// joining all the threads 
	for (int i =1 ; i <= Nthreads; i++){
		if(pthread_join(atm_threads[i-1], NULL) ){
			perror("Bank error: pthread_join failed");
		}
	}

	if (pthread_join(bank_commissions_thread, NULL)) {
		perror("Bank error: pthread_join failed");
	}
	
	// Wait for bank print thread to finish
	if (pthread_join(bank_print_Balance, NULL)) {
		perror("Bank error: pthread_join failed");
	}
	//*********************ending deleting/freeing and closeing everything that had been used********** 
	atm_vec.clear();
	delete[] atm_threads;
	//pthread_mutex_destroy(&log_lock);
	
	if (logFile.fail()) {
		perror("Bank error: close failed");
		exit(1);
	}
	
	return SUCCESS;
        
}



