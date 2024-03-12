
#include "Bank.hpp"
#include "ATM.hpp"
using namespace std;

ATM::ATM(int id) : ID(id) {}

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
    for (const auto& w : words) {
        if (arg == 1) {

        }
        arg++;
    }
}