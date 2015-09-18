#include "essentials.h"

/*
INBUILT COMMAND METHODS BEGIN
-all methods follow template specified by
    typedef void(*inbuiltCommandFunc)(vector<string>);

-each method must be added to the map inbuiltEngine::inbuiltMethods
-inside the constructor inbuiltEngine::inbuiltEngine()
-in order for it to be located during execution
*/

void echo ( vector<string> commandArguments ) {
    for ( int i = 1; i < commandArguments.size(); i++ ) {
        cout<<commandArguments[i]<<" ";
    } cout<<endl;
}

void pwd ( vector<string> commandArguments ) {
    cout<< getcwd( 0, 0 )<<endl;
}

void cd ( vector<string> commandArguments ) {
    if ( chdir( commandArguments[1].c_str() ) == -1 ) {
        perror(SHELLNAME);
    }
}

void pinfo ( vector<string> commandArguments ) {
    cout<<"PID: "<<getpid()<<endl;
}

/*
INBUILT COMMAND METHODS END
*/