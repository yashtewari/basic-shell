#include "essentials.h"

//command attributes
class commandDetails {
    public:
        vector<string> arguments;
        int largestArgument;
        commandDetails ( pair < vector<string>, int > inputPair ) {
            arguments = inputPair.first;
            largestArgument = inputPair.second;
        }
};