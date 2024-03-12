
#include "Bank.hpp"
#include "ATM.hpp"
#include "Acnt.hpp"
using namespace std;

Acnt::Acnt(int id, string pwd, int amt) : ID(id), password(pwd), amount(amt) {}

bool Acnt::comparePassword(string pwd) {
    return password == pwd;
}

int Acnt::updateAmount(int newAmount) {
    if (newAmount >= 0) {
        amount = newAmount;
        return 1;
    } else {
        return 0;
    }
}