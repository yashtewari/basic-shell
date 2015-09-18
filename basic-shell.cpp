#include<iostream>
#include<string>
#include<vector>
#include<cstdlib>
#include<cstdio>
#include<sstream>
#include<cstring>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<map>

#define SHELLNAME "ya-shell"
using namespace std;


class commandDetails {
    public:
        vector<string> arguments;
        int largestArgument;
        commandDetails ( pair < vector<string>, int > inputPair ) {
            arguments = inputPair.first;
            largestArgument = inputPair.second;
        }
};

char predefinedCommandsList[][50] = {"echo", "pwd", "cd", "pinfo"};

class inbuilt {
    public:
        vector<string> inbuiltList;
        map<string,int> inbuiltDict;
        
        inbuilt ( char predefinedCommands[][50] ) {
            //inbuiltList = predefinedCommands;
            //cout<<"constructor:\n";
            for ( int i = 0; predefinedCommands[i][0] != '\0'; i++ ) {
                //cout<<predefinedCommands[i]<<endl;
                inbuiltList.push_back( predefinedCommands[i] );
            }
            for ( int i = 1; i <= inbuiltList.size(); i++ ) {
                inbuiltDict[ inbuiltList[i-1] ] = i;
            }
        }

        bool isInbuilt ( string commandName ) {
            return ( inbuiltDict.find( commandName ) != inbuiltDict.end() );
        }

        int getCommandCode ( string commandName ) {
            return inbuiltDict[ commandName ];
        }

        void executeInbuilt ( vector<string> commandArguments ) {
            //cout<<"Executing from inbuilt:\n";
            //cout<<getCommandCode( commandArguments[0] )<<endl;
            switch ( getCommandCode( commandArguments[0] ) ) {
                case 1:
                    for ( int i = 1; i < commandArguments.size(); i++ ) {
                        cout<<commandArguments[i]<<" ";
                    } cout<<endl;
                    break;
                case 2:
                    cout<< getcwd( 0, 0 )<<endl;
                    break;
                case 3:
                    if ( chdir( commandArguments[1].c_str() ) == -1 ) {
                        perror(SHELLNAME);
                    }
                    break;
                case 4:
                    cout<<"PID: "<<getpid()<<endl;
                    break;
                default:
                    cout<<"Built-in command not found.\n";
            }
        }
} inbuiltCommands( predefinedCommandsList );

pair< vector<string>, int > splitString ( string line, char delim ) {
    istringstream splitStream( line );
    vector<string> output;
    string token;
    int maxLength = 0;
    while ( getline ( splitStream, token, delim ) ) {
        if ( token.length() ) {
            output.push_back ( token );
            if ( token.length() > maxLength ) {
                maxLength = token.length();
            }
        }
    }
    return make_pair( output, maxLength );
}

vector<const char*> convertVector ( vector<string> origVector ) {
    vector<const char *> finalVector; 
    finalVector.reserve( origVector.size()+1 );
    for ( int i = 0; i < origVector.size(); i++ ) {
        finalVector.push_back( origVector[i].c_str() );
    }
    finalVector.push_back( NULL );
    return finalVector;
}


bool executeCommand ( commandDetails command ) {
    if ( inbuiltCommands.isInbuilt( command.arguments[0] ) ) {
        inbuiltCommands.executeInbuilt( command.arguments );
        return true;
    }
    else {
        bool isBackgroundProcess = false;
        if ( command.arguments[ command.arguments.size()-1 ] == "&" ) {
            isBackgroundProcess = true;
            command.arguments.pop_back();
        }
        pid_t processId, waitId;
        int childStatus;

        vector<const char*> argumentsArray = convertVector( command.arguments );

        processId = fork();
        switch ( processId ) {
            case -1:
                perror(SHELLNAME);
                break;
            case 0:

                switch ( execvp ( argumentsArray[0], (char**) &argumentsArray[0] ) ) {
                    case -1:
                        perror(SHELLNAME);
                        exit( EXIT_FAILURE );
                    default:
                        if ( isBackgroundProcess ) {
                            if ( setpgid( 0, 0 ) == -1 ) {
                                perror(SHELLNAME);
                            }
                        }
                        exit( EXIT_SUCCESS );
                }
            default:
                if ( !isBackgroundProcess ) {
                    do {
                        waitId = waitpid( processId, &childStatus, WUNTRACED );
                    } while ( !WIFEXITED( childStatus ) && !WIFSIGNALED( childStatus ) );
                }
        }
    }
    return true;
}

string homeDirectory, currentDirectory, username, hostname;

string getCurrentDirectory() {
    currentDirectory = getcwd(0,0);
    string prefix( currentDirectory.begin(), currentDirectory.begin() + homeDirectory.length() );
    if ( prefix == homeDirectory ) {
        currentDirectory.erase( currentDirectory.begin(), currentDirectory.begin() + homeDirectory.length() );
        currentDirectory = "~" + currentDirectory;
    }
    return currentDirectory;
}

void initializeShell () {
        char temp[50];
        getlogin_r(temp,50); username = temp;
        gethostname(temp,50); hostname = temp;
        homeDirectory = currentDirectory = getcwd(0,0);
}

void mainShellLoop() {
    string input;
    vector<string> commandsList;
    bool status = true;

    while ( status ) {
        cout<<"<"<<username<<"@"<<hostname<<":"<<getCurrentDirectory()<<"> ";
        getline( cin , input );
        commandsList = splitString( input, ';' ).first;
        for ( int i = 0; i < commandsList.size(); i++ ) {
            commandDetails command = splitString( commandsList[i], ' ' );
            executeCommand( command );
        }
    }
}
        

int main ( int argc, char *argv[] ) {

    //main shell loop
    initializeShell();
    mainShellLoop();


    return EXIT_SUCCESS;
}
