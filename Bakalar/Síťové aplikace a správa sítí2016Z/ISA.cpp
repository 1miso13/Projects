#define DUBUG 0
#include <iostream>
#include <stdlib.h> 
#include <list>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "pomocneSubory/another.h"
#include "Parameters.h"
#include "socket/socket.h"
#include "Query.cpp"
using namespace std;
int spracovanie(int s,struct sockaddr_in *peer_addr,char *buffer,int bufferlen,socklen_t length,parameters* param);

int main(int argc,char*argv[])
{
	//  ./roughDNS [-m] [-h] [-p <port>] [<zonefile>]
	parameters *param = new parameters();
	int returnValue;
	if (( returnValue=parametry(argc,argv,param))==0)
	{
		//dalsie spracovanie
		if(DUBUG==1){
			cout<< "Nasleduje dalsie spracovanie"<<endl;
		}
		//run server
		int s;
		struct sockaddr_in server_addr;
		if((s=setSocket(&server_addr,param->port)) == -1 || bind(s, (struct sockaddr*)&server_addr, sizeof(server_addr) ) == -1){
			cerr<< "ERR_SOCKET"<<endl;
			returnValue=ERR_SOCKET;
		}
		else
		{
			while(true){
				memset(buffer, 0, BUFFER_LEN *sizeof (char));
				struct sockaddr_in peer_addr;
				socklen_t length= sizeof(peer_addr);
				int recvLen;
				if((recvLen=recvfrom(s,buffer,BUFFER_LEN,0,(struct sockaddr *) &peer_addr,&length))==-1)
				{
					cerr<< "chyba pri prijimani dat"<<endl;
					returnValue=ERR_RECV;
				}
				else
				{
					pid_t pid = fork();
					if (pid == 0)
					{
						// child process
						//zavolame funkciu ktora bude spracovavat
						returnValue=spracovanie(s,&peer_addr,buffer,recvLen,length,param)==0;
						break;
					}
					else if (pid > 0)
					{
						// parent process
					}
					else
					{
						// fork failed
						printf("Server: fork() failed!\n");
						returnValue= -11;
					}
				}


			}
		}

		


	}
	if (returnValue==1)
	{//bolo zadane -h
		returnValue=0;
	}
	delete param;
	return returnValue;
}

int spracovanie(int s,struct sockaddr_in *peer_addr,char *buffer,int bufferlen,socklen_t length,parameters * param){
	//
	int returnValue=0;
	
	Query *query = new Query(buffer,bufferlen);
	/*if(!query->testValid())
	{
		//chyba v prijatej sprave
		cerr<< "Chyba validity"<<endl;
		returnValue=-1;
	}
	else*/{

		//query->print();
		//cout<<endl;


		cout<< "q: "<< query->createQuestionOUT()<< endl;
		int Answerlength;
		char reply [512];
		query->createAnswer(param->zonefiles,reply,&Answerlength,param->ipAddress);
		//cout << "answer to send:" << Answerlength<<endl;
		//printStringInBiteForm(reply,Answerlength);
		if (strcmp(reply,"")==0)
		{
			cerr<<"Nastala chyba - Odpoved sa nepodarila vytvorit"<<endl;
			returnValue=-2;
		}
		else
		{
			//cout<< "r1: "<< reply << endl;
			if (sendto(s, reply, Answerlength, 0, (struct sockaddr*) peer_addr, length) == -1)
			{
				cerr<<"Nepodarilo sa odoslat spravu"<<endl;
				returnValue= -1;
			}
		}
	}
	delete query;
	
	return returnValue;
}