#include "essentials.h"
//get inbuilt methods in global space
#include "inbuilt-methods.h"

//function type to create std::map with
typedef void(*inbuiltCommandFunc)(vector<string>);

class inbuiltEngine {
    private:
        //std::map for quick reference
        map< string, inbuiltCommandFunc> commandMethods;

    public:
        //constructor
        inbuiltEngine () {
            //populate commandMethods
            commandMethods["echo"] = echo;
            commandMethods["pwd"] = pwd;
            commandMethods["cd"] = cd;
            commandMethods["pinfo"] = pinfo;
        }

        bool isInbuilt ( string commandName ) {
            //is commandName a commandMethods key?
            if ( commandMethods.find( commandName ) != commandMethods.end() )
                return true;
            else
                return false;
        }

        void execute ( vector<string> commandArguments ) {
            //pass the command arguments to the appropriate method
            //the 0th argument is the command name itself
            commandMethods[ commandArguments[0] ]( commandArguments );
        }

};