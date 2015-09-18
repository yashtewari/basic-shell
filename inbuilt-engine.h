#include "essentials.h"

class inbuilt {
    private:
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