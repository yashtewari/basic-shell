#include "essentials.h"
#ifndef __USEFUL_FUNCTIONS__ //only include once
#define __USEFUL_FUNCTIONS__

//split string 'line' using delimiter 'delim'
pair<vector<string>, int> splitString (string line, char delim) {
	//virtual input stream
    istringstream splitStream(line);
    vector<string> output;
    string token;
    int maxLength = 0;
    //while delim specified token from stream
    while (getline(splitStream, token, delim)) {
    	//if token is non-empty
        if (token.length()) {
            output.push_back(token);
            if (token.length() > maxLength) {
                maxLength = token.length();
            }
        }
    }
    //return pair(tokenized string, length of largest token)
    return make_pair(output, maxLength);
}

//convert a vector<string> into a vector<const char*>
//used for C-style functions
vector<const char*> convertVector (vector<string> origVector) {
    vector<const char *> finalVector; 
    finalVector.reserve(origVector.size()+1);
    for (int i = 0; i < origVector.size(); i++) {
        finalVector.push_back(origVector[i].c_str());
    }
    finalVector.push_back(NULL);
    return finalVector;
}

//get current directory in display format
//i.e. current directory with path relative from const string homeDirectory
string getCurrentDirectory() {
    //set current directory globally
    currentDirectory = getcwd(0,0);
    string prefix(currentDirectory.begin(), currentDirectory.begin() + homeDirectory.length());
    if (prefix == homeDirectory) {
        currentDirectory.erase(currentDirectory.begin(), currentDirectory.begin() + homeDirectory.length());
        currentDirectory = "~" + currentDirectory;
    }
    return currentDirectory;
}


#endif