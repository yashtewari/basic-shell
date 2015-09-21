#include "essentials.h"

#ifndef __COMMAND_DETAILS__ //only include once
#define __COMMAND_DETAILS__

//command attributes
class commandDetails {
    public:
        vector<string> arguments;
        int largestArgument;
        commandDetails (pair<vector<string>, int> inputPair) {
            arguments = inputPair.first;
            largestArgument = inputPair.second;
        }

        commandDetails () {}
};

#endif