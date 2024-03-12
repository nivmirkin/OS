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

#define SUCCESS 1
#define FAILE 0

using namespace std;

class Bank {
	vsctor<Accounts> AccountsList;
	int BankBalance;	
};

class Account{
	int ID;
	int PW[4];
	int balance ;
};

#endif