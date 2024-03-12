
#include "Bank.hpp"
#include "ATM.hpp"
#include <cstdlib>        
using namespace std;
ATM::ATM(int id) : id(id) {}

bool ATM::loadFile(const string& filePath) {
    fileHandle = fopen(filePath.c_str(), "r");
    return fileHandle == nullptr;
}

void ATM::ATMrun() {
    string line;
    while (getline(fileStream, line)) {
        // Process each line here
        cout << "ATM " << ID << " reading line: " << line << std::endl;
        if (!line.empty()) {
            istringstream iss(line);
            vector<string> words;
            string word;
            while (iss >> word) {
                words.push_back(word);
            }

            if (line[0] == 'O') {
                openAcc(words);
            }
            else if (line[0] == 'D') {

            }
            else if (line[0] == 'W') {

            }
            else if (line[0] == 'B') {

            }
            else if (line[0] == 'Q') {

            }
            else if (line[0] == 'T') {

            }
        }
    }
}

bool ATM::openAcc(vector<string> words) {
    int arg = 0;
    int id,amount;
    string pswd;
    for (const auto& w : words) {
        if (arg == 1) {
            id = atoi(w.c_str());
            if (isIDPresent(id)) {
                cerr << "Error " << getID() << ": Your transaction failed - account with the same id exists" << endl;
                return true;
            }
        }
        else if (arg == 2) {
            pswd = w;
        }
        else if (arg == 3) {
            amount = atoi(w.c_str());
        }
        arg++;
    }
    Account acnt(id,pswd,amount):
    accounts.emplace(id, acnt);
    cout << getID() << ": New account id is " << id << " with password "<< pswd <<" and initial balance "<<amount << endl;
    return false;

}

bool ATM::deposit(vector<string> words){
    int arg = 0;
    int id, amount;
    string pswd;
    Account* accptr;
    for (const auto& w : words) {
        if (arg == 1) {
            id = atoi(w.c_str());
            accptr = getAcntByID(id);

            if (acctr == nullptr) {
                cerr << "Error " << getID() << ": Your transaction failed - account id "<<id << " does not exist" << endl;
                return true;
            }

        }
        else if (arg == 2) {
            pswd = w;
            if (!accptr->comparePassword(pswd)) {
                cerr << "Error " << getID() << ": Your transaction failed - password for account id " << id << " is incorrect" << endl;
            }
        }
        else if (arg == 3) {
            amount = atoi(w.c_str());
        }
        arg++;
    }
    accptr->updateAmount(amount);

    cout << getID() << ": New account id is " << id << " with password " << pswd << " and initial balance " << amount << endl;
    return false;
}

int ATM::getID() {
    return this.id;
}



bool isIDPresent(int id) {
    return accounts.find(id) != accounts.end();
}

Account* getAcntByID(int id) {
    auto it = accounts.find(id);
    if (it != accounts.end()) {
        return &(it->second); // Return pointer to Acnt object
    }
    else {
        return nullptr; // ID not found in map
    }

