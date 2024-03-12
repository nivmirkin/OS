
#ifndef Acnt_HPP
#define Acnt_HPP

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <pthread.h>
#include <fstream>
#include <list>


using namespace std ;


#define SUCCESS 1
#define FAILE 0

class Acnt {
    int ID;
    string password;
    int amount;

public:
    Acnt(int id, string pwd, int amt);

    bool comparePassword(string pwd);

    int updateAmount(int newAmount);
};



#endif