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
bool decodeBegginingMessage(string str,string *type,string *name,int *size);

/**
 * test spravnsti parametrov a nasledny navrat cisla portu 
 * @return uspesnost
**/
bool testParameters(int argc,char*argv[],int *port){
	bool uspesnost=true;
	regex e("[0-9]*");
	if((argc !=3 && argc !=4) || strcmp(argv[1],"-p") || !regex_match(argv[2],e)){
		//chyba niesu spravne parametre
		cerr << "Server: server nebol spusteny so spravnimiA"<< endl;
		return false;
	}
	
	if(argc==4 ){
		if(!strcmp(argv[3],"-verbose")){
			vystup=true;
		}
		else{
			cerr << "Server: server nebol spusteny so spravnimiB"<< endl;
			return false;
		}
	}
	*port=stringToint(argv[2]);
	return uspesnost;
}

int spojenieAktivne(int clientSocket);

int main(int argc,char*argv[])
{
	int port;
	if(!testParameters(argc,argv,&port)){
		return -1;
	}
	if(vystup)
		cout<<"Server: Zadany port:\t"<<port<<endl;
	
    int server_socket;
	int peer_socket;
        struct sockaddr_in      server_addr;
        socklen_t       sock_len;
		server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket == -1)
        {
                fprintf(stderr, "Server: Error creating socket --> %s", strerror(errno));

                exit(EXIT_FAILURE);
        }
		
		memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        inet_pton(AF_INET, "127.0.0.1", &(server_addr.sin_addr));
        server_addr.sin_port = htons(port);
		// int no = 0;     
		// setsockopt(server_socket, IPPROTO_IPV6, IPV6_V6ONLY, (void *)&no, sizeof(no)); 
		if ((bind(server_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))) == -1)
        {
                fprintf(stderr, "Server: Error on bind --> %s", strerror(errno));

                exit(EXIT_FAILURE);
        }
		if ((listen(server_socket, 5)) == -1)
        {
                fprintf(stderr, "Server: Error on listen --> %s", strerror(errno));

                exit(EXIT_FAILURE);
        }
	for(int i =0;;i++)
	{
		
        struct sockaddr_in      peer_addr;
        
        sock_len = sizeof(struct sockaddr_in);
		peer_socket = accept(server_socket, (struct sockaddr *)&peer_addr, &sock_len);
        if (peer_socket == -1)
        {
                fprintf(stderr, "Server %d: Error on accept --> %s",i, strerror(errno));

                exit(EXIT_FAILURE);
        }
		
		
		//cakame kym sa niekto nepripoji
		if(vystup)
			cout << i<< endl;
		//vytvorime novy podproces
		
			pid_t pid = fork();
			if (pid == 0)
			{
				// child process
				close(server_socket);
				//zavolame funkciu ktora bude spracovavat stahovanie/odosielanie suborov
				int id;
				if((id=spojenieAktivne(peer_socket))!=0)
					cerr<<"Server: nastala chyba: "<<id<<endl;
				close(peer_socket);
				break;
			}
			else if (pid > 0)
			{
				close(peer_socket);
				// parent process
			}
			else
			{
				// fork failed
				printf("Server %d: fork() failed!\n",i);
				return 1;
			}
		
	}
	
	if(vystup)
		cout <<"Server: closing connection"<<endl;
	
	
	
	return 0;
}







int  spojenieAktivne(int clientSocket){
	if(vystup)
		cout<<"Server: prenos sa zacal"<<endl;
	string sprava;
	receiv(clientSocket,&sprava);
	if(vystup)
		cout<< "\n"<<sprava <<"\n"<<endl;
	
	string type;
	string fileName;
	int  fileSize;
	
	if(!decodeBegginingMessage(sprava,&type,&fileName,&fileSize)){
		cerr<<"Server: chyba dekodovania"<<endl;
		return -2;
	}
	
	if(isBegining(sprava,"UP\n")){
		fileName=getFileName(fileName);
		if(vystup)
			cout <<"Server: sending OK"<<endl;
		send(clientSocket,"OK");
		//zavolame funkciu ktora bude prijimat data a ukladat do suboru fileName
		if(vystup)
			cout <<"Server: cakanie na data"<<endl;
		sprava="";
		if(!receiv(clientSocket,&sprava,fileSize,"Uploading "+fileName)){
			cerr<<"Server: chyba prijimania dat"<<endl;
			return -3;
		}
		if(vystup)
			cout << "Server: sprava prijata"<<endl;
		//test ci sa nachadza OK\n na zaciatku + odstranenie
		if(!isBegining(sprava,"OK\n")){
			cerr<<"Server: chyba dat, chyba OK"<<endl;
		}
		sprava.erase(0,3);
		saveFile(fileName,sprava);
		if(vystup)
			cout <<"Server: sending second OK"<<endl;
		send(clientSocket,"OKJ");
		if(vystup)
			cout << "Server: Odoslane "<<endl;
	}
	else
	if(isBegining(sprava,"DOWN\n")){
		if(vystup)
			cout <<"Server: sending file: " << fileName <<endl;
		//odosle data zo suboru fileName (na zaciatku 'OK\r\n\r\n')
		string data;
		int length;
		if(!getFile(fileName,&data,&length)){
			data="FNF";//File Not Found
			send(clientSocket,data);
		}
		else{
			//odosielame data klientovi
			send(clientSocket,"OK\n"+to_string(length));
			receiv(clientSocket,&sprava);
			if(sprava!="OK"){
				cerr<< "nespravna odpoved od klienta: " << sprava << endl;
				return -20;
			}
			send(clientSocket,"OK\n"+data);
			
			
			
			
		}
	}
	else{
		//chyba prenosu
		cerr<<"Server: Chyba prenosu"<<endl;
		return -1;
		
	}
	if(vystup)
		cout <<"Server: qwerty " << fileName<<" " << fileSize<<endl;
	return 0;
}


bool decodeBegginingMessage(string str,string *type,string *name,int *size){
	try
	{
		*type = str.substr(0, str.find("\n", 0));
		
		str.erase(0, str.find("\n") + 1);
		*name = str.substr(0, str.find("\n", 0));
		
		str.erase(0, str.find("\n") + 1);
		if(str.length()>0)
		*size = stringToint(str.substr(0, str.find("\n", 0)));
		return true;
	}
	catch(...){
		
		return false;
	}
}


























