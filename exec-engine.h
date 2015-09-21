#include "essentials.h"
#include "command-details.h" //class specifying command attributes
#include "inbuilt-engine.h" //for inbuilt commands
#include "useful-functions.h"

class execEngine {
    private:
        inbuiltEngine inbuiltCommands; //object delivers all inbuilt functionality
        commandDetails command;
        bool isBackgroundProcess;
        pid_t processId, waitId;
        int childStatus;
        string inputFile, outputFile;
        bool createNewOutputFile;
        void parseCommand();
        void redirectionSetup();
        bool executeCommand(vector<const char*>);
    public:
        bool success;
        execEngine(commandDetails inputCommand) {
            success = false; //default
            command = inputCommand;

            parseCommand();

            //look for and execute if inbuilt
            if (inbuiltCommands.isInbuilt(command.arguments[0])) {
                inbuiltCommands.execute(command.arguments);
                success = true;
            }
            else {
                //pass C-friendly command arguments to execution method
                success = executeCommand(convertVector(command.arguments));
            }
        }
};

void execEngine::parseCommand() {
    //run in background?
    isBackgroundProcess = (command.arguments[ command.arguments.size()-1 ] == "&");
    //remove '&' from arguments
    if (isBackgroundProcess) command.arguments.pop_back();

    string tempFile; tempFile = inputFile = outputFile = "";
    for (int i = 0; i < command.arguments.size(); i++) {
        string arg = command.arguments[i];
        if (arg.substr(0,2) == ">>") {
            createNewOutputFile = false;
            if (arg.length() > 2) {
                outputFile = arg.substr(2);
                command.arguments.erase(command.arguments.begin() + i);
            }
            else {
                outputFile = command.arguments[i + 1];
                command.arguments.erase(command.arguments.begin() + i, command.arguments.begin() + i + 2);
            }
            i--;
        }
        else if (arg[0] == '<' || arg[0] == '>') {
            if (arg.length() > 1) {
                tempFile = arg.substr(1);
                command.arguments.erase(command.arguments.begin() + i);
            }
            else {
                tempFile = command.arguments[i + 1];
                command.arguments.erase(command.arguments.begin() + i, command.arguments.begin() + i + 2);
            }
            i--;
            switch (arg[0]) {
                case '<': inputFile = tempFile; break;
                case '>': outputFile = tempFile; createNewOutputFile = true; break;
            }
        }
    }
}

void execEngine::redirectionSetup() {
    int input, output;
    // cout<<"XXXXXXXXXXXXXX"<<inputFile<<"-"<<outputFile<<endl;
    if (inputFile != "") {
        input = open(inputFile.c_str(), O_RDONLY);
        //ADD FILE CHECK
        dup2(input, 0);
        close(input);
    }
    if (outputFile != "") {
        // cout<<createNewOutputFile<<endl;
        output = createNewOutputFile ? open(outputFile.c_str(), O_WRONLY | O_CREAT) : open(outputFile.c_str(), O_WRONLY | O_CREAT | O_APPEND);
        dup2(output, 1);
        close(output);
    }

}

bool execEngine::executeCommand(vector<const char*> argumentsArray) {
    processId = fork();
    switch (processId) {
        case -1: //fork unsuccessfull
            perror(SHELLNAME);
            break;
        case 0: //child process logic

            redirectionSetup();
            //execute in child
            switch (execvp (argumentsArray[0], (char**) &argumentsArray[0])) {
                case -1:
                    perror(SHELLNAME);
                    exit(EXIT_FAILURE);
                default:
                    //background process sets its ppid to init
                    if (isBackgroundProcess) {
                        if (setpgid(0, 0) == -1) {
                            perror(SHELLNAME);
                        }
                    }
                    exit(EXIT_SUCCESS);
            }
        default: //parent process logic
            //only wait for a foreground child
            if (!isBackgroundProcess) {
                do {
                    waitId = waitpid(processId, &childStatus, WUNTRACED);
                } while (!WIFEXITED(childStatus) && !WIFSIGNALED(childStatus));
            }
    }
    return true;
}
