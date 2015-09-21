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
        bool executeCommand(vector<const char*>);
    public:
        bool success;
        execEngine(commandDetails inputCommand) {
            success = false; //default
            command = inputCommand;
            //run in background?
            isBackgroundProcess = (command.arguments[ command.arguments.size()-1 ] == "&");
            //remove '&' from arguments
            if ( isBackgroundProcess ) command.arguments.pop_back();

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

bool execEngine::executeCommand (vector<const char*> argumentsArray) {
    processId = fork();
    switch (processId) {
        case -1: //fork unsuccessfull
            perror(SHELLNAME);
            break;
        case 0: //child process logic

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
