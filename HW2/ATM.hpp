
#ifndef ATM_HPP
#define ATM_HPP
#include "Bank.hpp"
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <pthread.h>
#include <fstream>
#include <list>
#include <vector>
#include <map>
class Bank;
extern Bank bank;

using namespace std ;


#define SUCCESS 1
#define FAIL 0

class ATM {
    int id;
    string filepath;

public:
    ATM(int id, string filePath);
    bool loadFile(const char* filePath);
    bool closeFile(void);
    static void* ATMrun(void* patm);

    bool openAcc(vector<string> words);

    bool deposit(vector<string> words);

    bool withdraw(vector<string> words);

    bool checkBalance(vector<string> words);

    bool closeAcc(vector<string> words);

    bool transfer(vector<string> words);


    int getID();
};


bool isIDPresent(int id); 
#endif