
#ifndef ATM_HPP
#define ATM_HPP

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <pthread.h>
#include <fstream>
#include <list>
#include <map>
extern map<int, Account> accounts;

using namespace std ;


#define SUCCESS 1
#define FAIL 0

class ATM {
    int id;
    FILE* fileHandle;

    bool loadFile(const string& filePath);

    void ATMrun();

    bool openAcc(vector<string> words);
    
    bool deposit(vector<string> words);

    int getID();

public:
    ATM(int id);
};


bool isIDPresent(int id); 
#endif