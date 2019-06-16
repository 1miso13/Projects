#include <iostream>
#include <string.h>
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
#include "../another/d.h"
using namespace std;
bool fileExists(string s){
	ifstream ifile(s);
	return ifile;
}
/**
 *
 *
 *	type - 	false 	download 
 *			true	upload 
 */
bool parameters(int argc,char*argv[],int *port,string *hostname,bool *type,string *filename)
{
	bool uspesnost=true;
	if(argc==7||argc==8){
		for(int i =1;i<argc;i=i+2)
		{
			if(!strcmp(argv[i],"-h")){
				*hostname=argv[i+1];
			}		
			else
			if(!strcmp(argv[i],"-p")){
				regex e("[0-9]*");
				if(regex_match(argv[i+1],e)){
					*port=stringToint(argv[i+1]);
				}
				else{
					cerr<<"Client: nespravne zadane cislo portu"<<endl;
					uspesnost=false;
				}
			}	
			else
			if(!strcmp(argv[i],"-u")||!strcmp(argv[i],"-d")){
				if(!strcmp(argv[i],"-u"))
					*type=true;
				else
					*type=false;
				*filename=argv[i+1];
			}
			else
			if(!strcmp(argv[i],"-verbose")){
				vystup=true;
			}
			else{
				cerr<<"Client: nespravne parametre"<<endl;
				uspesnost=false;
			}
			
		}
	}
	else{
		cerr<<"Client: nespravny pocet parametrov"<<endl;
		uspesnost=false;
	}
		
	return uspesnost;
}
                                                                                      

int main(int argc,char*argv[])
{
	pid_t pid = getpid();
	int port;
	string hostname;
	bool type;
	string filename;
	if(!parameters(argc,argv,&port,&hostname,&type,&filename)){
		return -1;
	}
	if(vystup)
	cout<<"Client "<<pid<<": Zadany port:\t"<<port<<endl
	    <<"Client "<<pid<<": Server:\t\t"<< hostname <<endl
	    <<"Client "<<pid<<": Filename:\t\t"<< filename <<endl
	    <<"Client "<<pid<<": Typ prenosu:\t"<<(type?"Upload":"Download") <<endl;
	
	//test ci existuje subor na odoslanie
	if(type){
		if(!fileExists(filename)){
			cerr<<"Client "<<pid<<": neexistujuci subor na odoslanie"<<endl;
			return-2;
		}
	}
	
	
    int client_socket;
	struct hostent *server;
    struct sockaddr_in server_address;
	if ((server = gethostbyname(hostname.c_str())) == NULL)
	{
        fprintf(stderr,"Client: ERROR %d: no such host as %s\n",pid, hostname.c_str());
        return EXIT_FAILURE;
    }
	
	bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
    server_address.sin_port = htons(port);
	
	
	if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
			fprintf(stderr, "Client: Error %d creating socket --> %s\n",pid, strerror(errno));

			return EXIT_FAILURE;
	}

	/* Connect to the server */
	if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
	{
			fprintf(stderr, "Client: Error %d on connect --> %s\n",pid, strerror(errno));

			return EXIT_FAILURE;
	}
	
	string sprava;
	if(type){
		//upload
		string data;
		int length;
		getFile(filename,&data,&length);
		send(client_socket,"UP\n"+filename+"\n"+to_string(data.size()));
		
		receiv(client_socket,&sprava);
		if(vystup)
			cout<<"Client: sdfvghjkmnbvc "<<sprava<<endl;
		if(sprava!="OK"){
			cerr<<"Client: nedosla ziadana odpoved: "<< sprava.length()<<endl;
			return -2;
		}
		sleep(2);
		if(vystup)
			cout << "Client: posielanie dat na server"<< endl;
		int s;
		if((s=send(client_socket,"OK\n"+data))<0){
			cerr<<"Client: Pri odosielani dat nastala chyba: "<<s<< endl;
			return -2;
		}
		if(vystup)
			cout <<"Client: data sent"<<endl;
		//odpoved od serveru ci dostal vsetky data 
		receiv(client_socket,&sprava);
		if(sprava!="OKJ"){
			cerr<<"Client: nedosla ziadana konecna odpoved: "<< sprava.length()<< sprava<<endl;
			return -2;
		}
		if(vystup)
			cout << "Client: prijal konecnu odpoved od servera." << endl;
			
	}
	else{
		//download
		send(client_socket,"DOWN\n"+filename);
		//prijmeme data
		receiv(client_socket,&sprava);
			
		if(sprava=="FNF"){//File Not Found
			cerr<<"Subor "<< filename << " nebol najdeny."<< endl;
			return 5;
		}
		else
		if(isBegining(sprava,"OK\n")){
			//pokracujeme v stahovani
			sprava.erase(0,3);//odstranenie hlavicky
			int fileSize=stringToint(sprava);
			send(client_socket,"OK");
			receiv(client_socket,&sprava,fileSize,"Downloading "+filename);
			if(!isBegining(sprava,"OK\n")){
				cerr<<"nespravny typ spravy";
				return -9;
			}
			sprava.erase(0,3);//odstranenie hlavicky
			saveFile(filename,sprava);
		}
		else{
			cerr<< "nespravny typ spravy"<<endl;
			return -10;
		}
		
	}
	
	
	
	
	
	
	
	close(client_socket);
	return 0;
}























