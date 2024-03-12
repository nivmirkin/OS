
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


using namespace std ;


#define SUCCESS 1
#define FAILE 0

class ATM {
	int ID;
	
	bool loadFile(const string& filePath) {
		fileHandle = fopen(filePath.c_str(), "r");
		if (!fileHandle) {
			return true;
		}
		return false;
	}
	
	void ATMrun(){
		string line;
		        while (getline(fileStream, line)) {
		            // Process each line here
		            cout << "ATM " << id << " reading line: " << line << std::endl;
		            if (!line.empty()){
		            	istringstream iss(line);
		            	vector<string> words;
		            	string word;
		            	while (iss >> word) {
		            		words.push_back(word);
		            	}
		            	
		            	if (line[0]=="O"){
		            		openAcc(words);
		            	}
		            	else if (line[0]=="D"){
		            		
		            	}
		            	else if (line[0]=="W"){
		            			            		
		            	}
		            	else if (line[0]=="B"){
		            			            		
		            	}
		            	else if (line[0]=="Q"){
		            			            		
		            	}
		            	else if (line[0]=="T"){
		            			            		
		            	}
		            	
		            }
				
		        }

	}
	bool openAcc(vector <string> words){
		int arg = 0;
		for (const auto& w : words) {
		     if (arg == 1){
		    	 
		     }
			arg++;
		}
	}
	
	private:
    	FILE *fileHandle;
};



#endif