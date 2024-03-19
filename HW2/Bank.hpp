#ifndef BANK_HPP
#define BANK_HPP

#include "ATM.hpp"
#include <fcntl.h>   
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <list>
#include <string>
#include <pthread.h>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <mutex>
#include <pthread.h>

#define SUCCESS 1
#define FAIL 0
#define WROND_PSWD -2
#define ACC_EXST -3
#define ACC_NOT_EXST -5
#define TO_ACC_NOT_EXST -6
#define INSFCNT_FUNDS -7

using namespace std;

class Account {
	int ID;
	string password;
	int amount;
	int read_cnt;
	pthread_mutex_t write_lock;
	pthread_mutex_t read_lock;

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
	map<int, Account*> accounts;

public:
	Bank();
	static void* bank_commissions(void* pbank);
	static void* print_stat();
	static void* bank_print_Balance();
	int addAcc(int id, string pswd, int amount);
	int removeAcc(int id, string pswd);
	int deposit(int id, string pswd, int amount);
	int withdraw(int id, string pswd, int amount);
	int checkBalance(int id, string pswd);
	int transer(int from_id, string pswd, int to_id, int amount, int* from_balance);
	int read_cnt;
	pthread_mutex_t	write_lock;
	pthread_mutex_t read_lock;
	void lock_write(void);
	void unlock_write(void);

	void lock_read(void);
	void unlock_read(void);


};




#endif