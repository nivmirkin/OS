
#include "Bank.hpp"
#include "ATM.hpp"

Account::Account(int id, string pwd, int amt) : ID(id), password(pwd), amount(amt) {}

bool Account::comparePassword(string pwd) {
    return password == pwd;
}

int Account::updateAmount(int newAmount) {
    if (newAmount >= 0) {
        amount = newAmount;
        return 1;
    }
    else {
        return 0;
    }
}



int main (int argc, char *argv[]) {

//check if there is no parameters given print error	
if(argc == 1){
	cerr << "Bank error: illegal arguments" << endl;
}




}

