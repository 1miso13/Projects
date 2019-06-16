#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fstream>
#include <sys/socket.h>
#include <sstream> 
#include <regex>
#include <climits>
#include <cctype>
#include <iostream>
#include <string.h>
#define BUFFER_SIZE 1024
#include <fstream>
using namespace std;
bool vystup=false;
bool isBegining(string String,string SubString);
bool isEnding(string String,string SubString);
string getFileName(string str);
int send(int clientSocket,string msg);
bool receiv(int clientSocket,int lenght,string *str);
int stringToint(string s);
#include "d.cpp"