
#include "ATM.hpp"
#include <cstdlib>        
using namespace std;
ATM::ATM(int id) : id(id) {}

bool ATM::loadFile(const string& filePath) {
    fileHandle = fopen(filePath.c_str(), "r");
    return fileHandle == nullptr;
}

bool ATM::closeFile(void) {
    if (fileHandle != nullptr) {
        fclose(fileHandle);
        fileHandle = nullptr;
    }
}

static void* ATM::ATMrun(void* patm) {
    string line;
    ATM* atm = static_cast<ATM*>(patm);
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
                atm->transfer(T);
            }
        }
    }
}

bool ATM::openAcc(vector<string> words) {
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
        cerr << "Error " << getID() << ": Your transaction failed - account with the same id exists" << endl;
        return true;
    }
    else if (res == SUCCESS) {
        cout << getID() << ": New account id is " << id << " with password " << pswd << " and initial balance " << amount << endl;
        return false;
    }
}

bool ATM::deposit(vector<string> words){
    int arg = 0;
    int id, amount;
    string pswd;
    Account* accptr;
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
       cerr << "Error " << getID() << ": Your transaction failed - account id " << id << " does not exist" << endl
       return true;

    }
    else if (res == WROND_PSWD) {
        cerr << "Error " << getID() << ": Your transaction failed - password for account id " << id << " is incorrect" << endl
        return true;

    }
    else if (res >= 0) {
        cout << getID() << ": Account " << id << " new balance is " << res << " after " << amount << " $ was deposited" << 
        return false;
    }

}


bool ATM::withdraw(vector<string> words) {
    int arg = 0;
    int id, amount,res;
    string pswd;
    Account* accptr;
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
        cerr << "Error " << getID() << ": Your transaction failed - account id " << id << " does not exist" << endl;
        return true;
    }
    else if (res == WROND_PSWD) {
        cerr << "Error " << getID() << ": Your transaction failed - password for account id " << id << " is incorrect" << endl;
        return true;

    }
    else if (res == INSFCNT_FUNDS) {
        cerr << "Error " << getID() << ": Your transaction failed - account id " << id << " balance is lower than amount " << amount << endl;
        return true;
    }
    else if (res >= 0) {
        cout << getID() << ": Account " << id << " new balance is " << res << " after " << amount << " $ was withdrew" << endl;
        return false;
    }

}

bool ATM::checkBalance(vector<string> words) {
    int arg = 0;
    int id, amount, res;
    string pswd;
    Account* accptr;
    for (const auto& w : words) {
        if (arg == 1) {
            id = atoi(w.c_str()
        }
        else if (arg == 2) {
            pswd = w;
        }
        arg++;
    }
    res = bank.checkBalance(id,pswd);
    if (res == ACC_NOT_EXST) {
        cerr << "Error " << getID() << ": Your transaction failed - account id " << id << " does not exist" << endl;
        return true;
    }
    else if (res == WROND_PSWD) {
        cerr << "Error " << getID() << ": Your transaction failed - password for account id " << id << " is incorrect" << endl;
        return true;
    }
    else if (res >= 0) {
        cout << getID() << ": Account " << id << " balance is " << res << endl;
        return false;
    }
}

bool ATM::closeAcc(vector<string> words) {
    int arg = 0;
    int id, amount, res;
    string pswd;
    Account* accptr;
    for (const auto& w : words) {
        if (arg == 1) {
            id = atoi(w.c_str());
        }
        else if (arg == 2) {
            pswd = w;
        }
        arg++;
    }
    res = back.removeAcc(id, pswd);
    if (res == ACC_NOT_EXST) {
        cerr << "Error " << getID() << ": Your transaction failed - account id " << id << " does not exist" << endl;
        return true;
    }
    else if (res == WROND_PSWD){
        cerr << "Error " << getID() << ": Your transaction failed - password for account id " << id << " is incorrect" << endl;
        return true;
    }
    else if (res > 0) {
        cout << getID() << ": Account " << id << " is now closed. Balance was " << res << endl;
        return false;
    }
}

bool ATM::transfer(vector<string> words) {
    int arg = 0;
    int from_id,to_id, amount, res;
    string pswd;
    Account* accptr;
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
    res = back.transer(from_id, pswd, to_id,amount, &from_balance);
    if (res == ACC_NOT_EXST) {
        cerr << "Error " << getID() << ": Your transaction failed - account id " << from_id << " does not exist" << endl;
        return true;
    }
    else if (res == TO_ACC_NOT_EXST) {
        cerr << "Error " << getID() << ": Your transaction failed - account id " << to_id << " does not exist" << endl;
        return true;
    }
    else if (res == WROND_PSWD) {
        cerr << "Error " << getID() << ": Your transaction failed - password for account id " << from_id << " is incorrect" << endl;
        return true;
    }
    else if (res == INSFCNT_FUNDS) {
        cerr << "Error " << getID() << ": Your transaction failed - account id " << from_id << " balance is lower than amount " << amount << endl;
        return true;
    }
    else if (res >= 0) {
        cout << getID() << ": Transfer " << amount << :" from account " << from_id << " to account " << to_id << " new account balance is " << from_balance << " new target account balance is " << res << endl;
        return false;
    }
}


int ATM::getID() {
    return this.id;
}



bool isIDPresent(int id) {
    return bank.accounts.find(id) != bank.accounts.end();
}

Account* getAcntByID(int id) {
    auto it = bank.accounts.find(id);
    if (it != bank.accounts.end()) {
        return &(it->second); // Return pointer to Acnt object
    }
    else {
        return nullptr; // ID not found in map
    }

