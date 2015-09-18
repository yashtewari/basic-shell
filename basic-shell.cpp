#include "essentials.h"
#include "inbuilt-engine.h" //for inbuilt commands
#include "command-details.h" //class specifying command attributes
#include "useful-functions.h"

inbuiltEngine inbuiltCommands; //object delivers all inbuilt functionality

bool executeCommand (commandDetails command) {
    if (inbuiltCommands.isInbuilt(command.arguments[0])) {
        inbuiltCommands.execute(command.arguments);
        return true;
    }
    else {
        bool isBackgroundProcess = false;
        if (command.arguments[ command.arguments.size()-1 ] == "&") {
            isBackgroundProcess = true;
            command.arguments.pop_back();
        }
        pid_t processId, waitId;
        int childStatus;

        vector<const char*> argumentsArray = convertVector(command.arguments);

        processId = fork();
        switch (processId) {
            case -1:
                perror(SHELLNAME);
                break;
            case 0:

                switch (execvp (argumentsArray[0], (char**) &argumentsArray[0])) {
                    case -1:
                        perror(SHELLNAME);
                        exit(EXIT_FAILURE);
                    default:
                        if (isBackgroundProcess) {
                            if (setpgid(0, 0) == -1) {
                                perror(SHELLNAME);
                            }
                        }
                        exit(EXIT_SUCCESS);
                }
            default:
                if (!isBackgroundProcess) {
                    do {
                        waitId = waitpid(processId, &childStatus, WUNTRACED);
                    } while (!WIFEXITED(childStatus) && !WIFSIGNALED(childStatus));
                }
        }
    }
    return true;
}


void initializeShell () {
        char temp[50];
        getlogin_r(temp,50); username = temp;
        gethostname(temp,50); hostname = temp;
        homeDirectory = getcwd(0,0);
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
            executeCommand(command);
        }
    }
}
        

int main (int argc, char *argv[]) {

    //main shell loop
    initializeShell();
    mainShellLoop();


    return EXIT_SUCCESS;
}
