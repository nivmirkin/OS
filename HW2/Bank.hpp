#ifndef BANK_HPP
#define BANK_HPP

#include "ATM.hpp"
#include <fcntl.h>   
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <pthread.h>
#include <cmath>
#include <algorithm>
#include <mutex>
#include <pthread.h>
#include <map>
#define SUCCESS 1
#define FAIL 0
#define WROND_PSWD -2
#define ACC_EXST -3
#define ACC_NOT_EXST -5
#define TO_ACC_NOT_EXST -6
#define INSFCNT_FUNDS -7

using namespace std;
extern ofstream logFile;
class Account {
	int ID;
	string password;
	int amount;
	int read_cnt;
	pthread_mutex_t acc_write_lock;
	pthread_mutex_t acc_read_lock;

public:
	Account(int id, string pwd, int amt);
	~Account();

	void lock_write(void);
	void unlock_write(void);

	void lock_read(void);
	void unlock_read(void);

	bool comparePassword(string pwd);

	int updateAmount(int addedAmount);
	int cmsnWithdraw(int per);
	int getAmount();
	int getAmount(string* pW);

	
};


class Bank {
	int BankBalance;
	int read_cnt;
	bool ATMsRunning;
	pthread_mutex_t log_lock;
	pthread_mutex_t	write_lock;
	pthread_mutex_t read_lock;
public:
	pthread_mutex_t bankBalanceLock;
	map<int, Account*> accounts;
	
	Bank();
	~Bank();
	static void* bank_commissions(void* pbank);
	static void* bank_print_Balance(void* pbank);
	void print_stat(void);
	int addAcc(int id, string pswd, int amount);
	int removeAcc(int id, string pswd);
	int deposit(int id, string pswd, int amount);
	int withdraw(int id, string pswd, int amount);
	int checkBalance(int id, string pswd);
	int transer(int from_id, string pswd, int to_id, int amount, int* from_balance);
	void lock_write(void);
	void unlock_write(void);

	void lock_read(void);
	void unlock_read(void);

	void writeLog(ostringstream* buff);
	bool ATMsStatus(void);
	void ATMsStart(void);
	void ATMsFinish(void);


};




#endif