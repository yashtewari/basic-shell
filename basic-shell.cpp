#include "essentials.h"
#include "command-details.h" //class specifying command attributes
#include "useful-functions.h"
#include "exec-engine.h"
#include "exits-handler.h"

void initializeShell () {
        char temp[50];
        getlogin_r(temp,50); username = temp;
        gethostname(temp,50); hostname = temp;
        homeDirectory = currentDirectory = getcwd(0,0);

        // class instance handles all child process exits
        exitsHandler shellExitsHandler;
}

void mainShellLoop() {
    string input;
    vector<string> commandsList;
    bool status = true;

    while (status) {
        cout<<"<"<<username<<"@"<<hostname<<":"<<getCurrentDirectory()<<"> ";
        getline(cin , input);
        commandsList = splitString(input, ';').first;
        for (int i = 0; i < commandsList.size(); i++) {
            commandDetails command = splitString(commandsList[i], ' ');
            execEngine commandExecution(command);
            // Add link to command processor
        }
    }
}
        

int main (int argc, char *argv[]) {

    //main shell loop
    initializeShell();
    mainShellLoop();

    return EXIT_SUCCESS;
}
