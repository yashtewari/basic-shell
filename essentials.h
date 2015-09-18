#ifndef __ESSENTIALS__ //only include once
#define __ESSENTIALS__

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <map>

#define SHELLNAME "ya-shell"
using namespace std;

//inbuilt commands
char predefinedCommandsList[][50] = {"echo", "pwd", "cd", "pinfo"};

#endif