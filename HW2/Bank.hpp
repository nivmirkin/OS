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
#define FAILE 0

using namespace std;

class Bank {
	int BankBalance;	
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