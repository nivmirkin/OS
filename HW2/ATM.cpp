
#include "ATM.hpp"
#include <cstdlib>        
using namespace std;
ATM::ATM(int id, string filePath) : id(id), filepath(filePath) {}
//ATM thread function - reads input and calls fitting function
void* ATM::ATMrun(void* patm) {
    bank.ATMsStart();
    string line;
    ATM* atm = static_cast<ATM*>(patm);
    ifstream filehandle(atm->filepath);
    if (!filehandle.is_open()) {
        perror("Bank error: open failed");
        return nullptr;
    }
    while (getline(filehandle, line)) {
        usleep(100000);
        // Process each line here
        ostringstream buff;
        buff << "ATM " << atm->getID() << " reading line: " << line << std::endl;
        bank.writeLog(&buff);
        if (!line.empty()) {
            istringstream iss(line);
            vector<string> words;
            string word;
            while (iss >> word) {
                words.push_back(word);
            }
            if (line[0] == 'O') {
                atm->openAcc(words);
            }
            else if (line[0] == 'D') {
                atm->deposit(words);
            }
            else if (line[0] == 'W') {
                atm->withdraw(words);
            }
            else if (line[0] == 'B') {
                atm->checkBalance(words);
            }
            else if (line[0] == 'Q') {
                atm->closeAcc(words);
            }
            else if (line[0] == 'T') {
                atm->transfer(words);
            }
        }
    }
    filehandle.close();
    return nullptr;
}
//  Operation function - parsing input and calling bank function
bool ATM::openAcc(vector<string> words) {
    ostringstream buff;
    int arg = 0;
    int id,amount,res;
    string pswd;
    for (const auto& w : words) {
        if (arg == 1) {
            id = atoi(w.c_str());            
           
        }
        else if (arg == 2) {
            pswd = w;
        }
        else if (arg == 3) {
            amount = atoi(w.c_str());
        }
        arg++;
    }
    res = bank.addAcc(id, pswd, amount);
    if (res == ACC_EXST){
        buff << "Error " << getID() << ": Your transaction failed - account with the same id exists" << endl;
        bank.writeLog(&buff);
        return true;
    }
    else if (res == SUCCESS) {
        buff << getID() << ": New account id is " << id << " with password " << pswd << " and initial balance " << amount << endl;
        bank.writeLog(&buff);
        return false;
    return true;
    }
    return true;
}

bool ATM::deposit(vector<string> words){
    int arg = 0;
    int id, amount;
    ostringstream buff;
    string pswd;
    for (const auto& w : words) {
        if (arg == 1) {
            id = atoi(w.c_str());
        }
        else if (arg == 2) {
            pswd = w;
        }
        else if (arg == 3) {
            amount = atoi(w.c_str());
        }
        arg++;
    }
    int res = bank.deposit(id, pswd, amount);
    if (res == ACC_NOT_EXST) {
       buff << "Error " << getID() << ": Your transaction failed - account id " << id << " does not exist" << endl;
       bank.writeLog(&buff);
       return true;

    }
    else if (res == WROND_PSWD) {
        buff << "Error " << getID() << ": Your transaction failed - password for account id " << id << " is incorrect" << endl;
        bank.writeLog(&buff);
        return true;

    }
    else if (res >= 0) {
        buff << getID() << ": Account " << id << " new balance is " << res << " after " << amount << " $ was deposited" << endl;
        bank.writeLog(&buff);
        return false;
    }
    return true;

}


bool ATM::withdraw(vector<string> words) {
    int arg = 0;
    int id, amount,res;
    string pswd;
    ostringstream buff;
    for (const auto& w : words) {
        if (arg == 1) {
            id = atoi(w.c_str());
        }
        else if (arg == 2) {
            pswd = w;
        }
        else if (arg == 3) {
            amount = atoi(w.c_str());
        }
        arg++;

    } 
    res = bank.withdraw(id, pswd, amount);
    if (res == ACC_NOT_EXST) {
        buff << "Error " << getID() << ": Your transaction failed - account id " << id << " does not exist" << endl;
        bank.writeLog(&buff);
        return true;
    }
    else if (res == WROND_PSWD) {
        buff << "Error " << getID() << ": Your transaction failed - password for account id " << id << " is incorrect" << endl;
        bank.writeLog(&buff);
        return true;

    }
    else if (res == INSFCNT_FUNDS) {
        buff << "Error " << getID() << ": Your transaction failed - account id " << id << " balance is lower than amount " << amount << endl;
        bank.writeLog(&buff);
        return true;
    }
    else if (res >= 0) {
        buff << getID() << ": Account " << id << " new balance is " << res << " after " << amount << " $ was withdrew" << endl;
        bank.writeLog(&buff);
        return false;
    }
    return true;

}

bool ATM::checkBalance(vector<string> words) {
    int arg = 0;
    int id, res;
    string pswd;
    ostringstream buff;
    for (const auto& w : words) {
        if (arg == 1) {
            id = atoi(w.c_str());
        }
        else if (arg == 2) {
            pswd = w;
        }
        arg++;
    }
    res = bank.checkBalance(id,pswd);
    if (res == ACC_NOT_EXST) {
        buff << "Error " << getID() << ": Your transaction failed - account id " << id << " does not exist" << endl;
        bank.writeLog(&buff);
        return true;
    }
    else if (res == WROND_PSWD) {
        buff << "Error " << getID() << ": Your transaction failed - password for account id " << id << " is incorrect" << endl;
        bank.writeLog(&buff);
        return true;
    }
    else if (res >= 0) {
        buff << getID() << ": Account " << id << " balance is " << res << endl;
        bank.writeLog(&buff);
        return false;
    }
    return true;

}

bool ATM::closeAcc(vector<string> words) {
    int arg = 0;
    int id, res;
    string pswd;
    ostringstream buff;
    for (const auto& w : words) {
        if (arg == 1) {
            id = atoi(w.c_str());
        }
        else if (arg == 2) {
            pswd = w;
        }
        arg++;
    }
    res = bank.removeAcc(id, pswd);
    if (res == ACC_NOT_EXST) {
        buff << "Error " << getID() << ": Your transaction failed - account id " << id << " does not exist" << endl;
        bank.writeLog(&buff);
        return true;
    }
    else if (res == WROND_PSWD){
        buff << "Error " << getID() << ": Your transaction failed - password for account id " << id << " is incorrect" << endl;
        bank.writeLog(&buff);
        return true;
    }
    else if (res > 0) {
        buff << getID() << ": Account " << id << " is now closed. Balance was " << res << endl;
        bank.writeLog(&buff);
        return false;
    }
    return true;

}

bool ATM::transfer(vector<string> words) {
    int arg = 0;
    int from_id,to_id, amount, res;
    string pswd;
    ostringstream buff;
    for (const auto& w : words) {
        if (arg == 1) {
            from_id = atoi(w.c_str());
        }
        else if (arg == 2) {
            pswd = w;
        }
        else if (arg == 3) {
            to_id = atoi(w.c_str());
        }
        else if (arg == 4) {
            amount = atoi(w.c_str());
        }
        arg++;
    }
    int from_balance;
    res = bank.transer(from_id, pswd, to_id,amount, &from_balance);
    if (res == ACC_NOT_EXST) {
        buff << "Error " << getID() << ": Your transaction failed - account id " << from_id << " does not exist" << endl;
        bank.writeLog(&buff);
        return true;
    }
    else if (res == TO_ACC_NOT_EXST) {
        buff << "Error " << getID() << ": Your transaction failed - account id " << to_id << " does not exist" << endl;
        bank.writeLog(&buff);
        return true;
    }
    else if (res == WROND_PSWD) {
        buff << "Error " << getID() << ": Your transaction failed - password for account id " << from_id << " is incorrect" << endl;
        bank.writeLog(&buff);
        return true;
    }
    else if (res == INSFCNT_FUNDS) {
        buff << "Error " << getID() << ": Your transaction failed - account id " << from_id << " balance is lower than amount " << amount << endl;
        bank.writeLog(&buff);
        return true;
    }
    else if (res >= 0) {
        buff << getID() << ": Transfer " << amount << " from account " << from_id << " to account " << to_id << " new account balance is " << from_balance << " new target account balance is " << res << endl;
        bank.writeLog(&buff);
        return false;
    }
    return true;
}


int ATM::getID() {
    return this->id;
}



bool isIDPresent(int id) {
    return bank.accounts.find(id) != bank.accounts.end();
}

Account* getAcntByID(int id) {
    auto it = bank.accounts.find(id);
    if (it != bank.accounts.end()) {
        return (it->second); // Return pointer to Acnt object
    }
    else {
        return nullptr; // ID not found in map
    }
}

