#ifndef SYMBOL
#define _PARAM_ 

#define HELP 1
#define ERR_HELP -20
#define ERR_COUNT_OF_PARAM -21
#define ERR_ZLE_ZADANY_PORT -22
#define ERR_ZONE_FILE -23
#define ERR_MITM_ADDRESS -24
#define VALID 0
#include <iostream>
#include <stdlib.h> 
#include <vector>
#include <stdio.h>
#include <string.h>
#include <fstream> 
#include <arpa/inet.h>
using namespace std;
class parameters {
public:
	parameters(void);
	int port;
	bool p;
	bool m;
    string ipAddress;
	vector<string> zonefiles;
}; 

int parametry(int argc,char*argv[],parameters *param);
int setPort(int argc,char*argv[],parameters *param,int *i);
#include "Parameters.cpp"

#endif