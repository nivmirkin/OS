#ifndef BANK_HPP
#define BANK_HPP

#include "ATM.hpp"
#include "bank.hpp"
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
#define INSFCNT_FUNDS -7

using namespace std;

class Bank {
	int BankBalance;
	map<int, Account*> accounts;

public:
	int updating_BankBalance(int Commission_Percent);
	int bank_commissions_thread(void);

	int addAcc(int id, string pswd, int amount);
	int removeAcc(int id, string pswd);
	int deposit(int id, string pswd, int amount);
	int withdraw(int id, string pswd, int amount);
	int checkBalance(int id, string pswd);
	int read_cnt;
	mutex write_lock;
	mutex read_lock;
	void lock_write(void);
	void unlock_write(void);

	void lock_read(void);
	void unlock_read(void);


};

class Account{
	int ID;
	string PW;
	int amount;
	int read_cnt;
	mutex write_lock;
	mutex read_lock;
	
public:
	Account(int id, string pwd, int amt);
	~Account();

	void lock_write(void);
	void unlock_write(void);

	void lock_read(void);
	void unlock_read(void);

	bool comparePassword(string pwd);

	int updateAmount(int addedAmount);
	int getAmount();
};


#endif