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
#define EXIT_FAILURE 1
#define PORT_NUM 80
#define strnicmp strncasecmp
#define MAX_COUNT_OF_HOPS 5
#define debug 0
using namespace std;
int stringToint(string s);
char* intToCharArray(int s);
char* stringToCharArray(string s);
string getFileName(string cestaKsuboru);
char* message(string addres, string cesta,int http,bool head);
int getCode(string buf,int *type,string *location,bool *chunked);
bool sss(string retazec,string* addr,string* cestuKSuboru, int* port );
bool isBeginning(string String,string substring);
int zmenaArgumentovNaString(int argc,char*argv[],string *String);
int main(int argc,char*argv[])
{
    string hostname;
	struct hostent *server;
    struct sockaddr_in server_address;
	if(debug==1)
	{
		cout<<argv[1]<<endl;
	}
    if(zmenaArgumentovNaString(argc,argv,&hostname))
    {
        cerr<<"Nebola spravne zadana adresa."<<endl;
        return 1;
    }
	if(debug==1)
	{
		cout<<"Adresa:"<<hostname<<endl;
	}
	string addr;
	string cestuKSuboru;
	int port;
	if(!sss( hostname, &addr, &cestuKSuboru ,&port))
		return EXIT_FAILURE;
	if(debug==1)
	{
		cout<<"addr:"<<addr<<endl;
		cout<<"cesta:"<<cestuKSuboru<<endl;
	}
	string FileName = getFileName(cestuKSuboru);
	if ((server = gethostbyname(addr.c_str())) == NULL)
	{
        fprintf(stderr,"ERROR: no such host as %s\n", addr.c_str());
        return EXIT_FAILURE;
    }
	
	bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
    server_address.sin_port = htons(port);
	
    int clientSocket;
    if((clientSocket = socket(AF_INET, SOCK_STREAM,0))<0)
    {
		perror("ERROR: socket");
        return EXIT_FAILURE;
    }
	
	
	
	if(connect(clientSocket,(const struct sockaddr *) &server_address, sizeof(server_address)) != 0)
	{
		perror("ERROR: connect");

        return EXIT_FAILURE;

    }
	int type=1;
	// send the HTTP HEAD request
	const char* msg = message(addr, cestuKSuboru,type,true);//"HEAD / HTTP/1.1\r\nHost: www.google.com\r\nConnection: close\r\n\r\n";
	send(clientSocket, msg, strlen(msg), 0);
	
	char buf[1024];
	memset(buf, 0, 1024);
	int n=1;
	int pocetPresmerovani=0;
	int code=1;
	int len = strlen(buf);
	bool chunked=false;
	string location;
	for(int countOfJumps=0;n&&countOfJumps<MAX_COUNT_OF_HOPS;countOfJumps++)
	{	
		if(debug==1)
		{	
			cout <<"return code:" <<code<<"     "<< n<< endl;
		}
		if(code==200){ //v poriadku
			n=0;
			if(debug==1)
			{		
				cout<<"adresa"<<addr;
			}
			msg = message(addr, cestuKSuboru,type,false);
			if(debug==1)
			{		
				cout << msg<<endl;
			}
			send(clientSocket, msg, strlen(msg), 0);
			int ffff;
			string data="";
			
			memset(buf, 0, 1024 *sizeof (char));
			while((ffff=recv(clientSocket, buf, 1024, 0))>0){
				if(ffff==-1)
				{
					return EXIT_FAILURE;
				}
				if(debug==1)
				{		
					cout<<ffff<<endl;
				}
				for(int i=0;i<ffff;i++)
				{
					data+=buf[i];
				}
			/*	if(debug==1)
				{		
					data.append(buf);
				}*/
				
			}
			int index;
			if ((index = data.find("\r\n\r\n")) != string::npos)
			{
				string Hlavicka=data.substr(0,index);
				if(Hlavicka.find("Transfer-Encoding: chunked\r\n") != string::npos)
					chunked=true;
				else
					chunked=false;
				data.erase(0, index+4);
			}
			string hexa="",aux="";
			size_t ind;
			long int number = -1;
			if (chunked == 1) {
				while (number != 0) {
					if ((ind = data.find("\r\n")) != string::npos) {
						//get number
						hexa = data.substr(0, ind);
						hexa.append("\0");
						number = strtol(hexa.c_str(), NULL, 16);
						//delete crlf
						data.erase(0, ind+2);
						//copy "number" of characters
						aux.append(data, 0, number);
						//delete crlf
						data.erase(0, number+2);
					}
				}
				data = aux;
			}
			
			
			
			ofstream myfile;
			if(debug==1)
			{		
				cout <<"Zapis do suboru: "<<FileName<<endl;
			}
			myfile.open (FileName);
			myfile<<data;
			myfile.close();
		}
		else
		if(code== 302||code==301){ //Permanent redirect
			if(debug==1)
			{		
				cout << "presmerovanie na:" << location<< endl<<endl;
			}
			if(!sss( location, &addr, &cestuKSuboru,&port))
				return EXIT_FAILURE;
			if ((server = gethostbyname(addr.c_str())) == NULL)
			{
				fprintf(stderr,"ERROR: no such host as %s\n", addr.c_str());
				return EXIT_FAILURE;
			}
			bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
			server_address.sin_port = htons(port);
				msg = message(addr, cestuKSuboru,type,true);
			if(debug==1)
			{		
				cout<<"debug "<< endl;
			}
			pocetPresmerovani++;
			close(clientSocket);
			if(debug==1)
			{		
				cout<<"addr:"<<addr<<endl;
				cout<<"cesta:"<<cestuKSuboru<<endl;
			}
			if((clientSocket = socket(AF_INET, SOCK_STREAM,0))<0)
			{
				perror("ERROR: socket");
				return EXIT_FAILURE;
			}
			if(connect(clientSocket,(const struct sockaddr *) &server_address, sizeof(server_address)) != 0)
			{
				perror("ERROR: connect");

				return EXIT_FAILURE;

			}
			send(clientSocket, msg, strlen(msg), 0);
			
			memset(buf, 0, 1024);
			int ffff;
			if(recv(clientSocket, buf, 1024, 0)==-1)
			{
				return EXIT_FAILURE;
			}
		/*{
				if(debug==1)
				{
					cout<<ffff;
				}
				if(ffff==-1)
				{
					return EXIT_FAILURE;
				}
			}*/
			if(debug==1)
			{		
				cout<< buf<<endl;
			}
			
			code = getCode(buf,&type,&location,&chunked);
		}
		else
		if(code>=400&& code<600)
		{
			cerr << code<<endl;
		}
		else
		{
			int ffff;
			memset(buf, 0, 1024);
			recv(clientSocket, buf, 1024, 0);
			/*{
				if(debug==1)
				{
					cout<<ffff;
				}
				if(ffff==-1)
				{
					return EXIT_FAILURE;
				}
			}*/
			if(debug==1)
			{		
				cout<< buf<<endl;
			}
			
			code = getCode(buf,&type,&location, &chunked);
		}
			
		
		
	}
	
	
	close(clientSocket);
	
    return 0;
}
int zmenaArgumentovNaString(int argc,char*argv[],string *String)
{
    int err=0;
    if(argc==1)
    {//nebola zadana adresa
        err=1;
    }
    else
    {//spajame jednotlive argumenty do jedneho retazca
        *String=argv[1];
        if(debug==1)
		{		
			cout<< "pocet je: " << argc << endl;
		}
        int index;
		while ((index = (*String).find(" ")) != string::npos)
			(*String).replace(index, 1, "%20");
    }
    return err;
}
//odstrani http:// , adresu xxx.xx  cestu k suboru xxx/xxx/xx.xx
bool sss(string retazec,string* addr,string* cestaKSuboru, int *port )
{
	const char* http  ="http://";
	if(isBeginning(retazec,http))
	{
		retazec=retazec.substr(strlen(http));
	}
	else 
	{
		return false;
	}
	if(retazec.find("/") != std::string::npos){
		*cestaKSuboru 	=retazec.substr(retazec.find("/"), 	retazec.length()-retazec.find("/")		);
		int otaznik,mriezka;
		if((otaznik=(*cestaKSuboru).find('?') )!= string::npos||(mriezka=(*cestaKSuboru).find('|')) != std::string::npos)
		{
			if(otaznik>mriezka)
				*cestaKSuboru=(*cestaKSuboru).erase(mriezka);
			else
				*cestaKSuboru=(*cestaKSuboru).erase(otaznik);
		}
			
	}
	else
		*cestaKSuboru +="/";
	*addr 			=retazec.substr(0,					retazec.find("/")		);
	int index=0;
	if((index=(*addr).find(":")) != std::string::npos){
	if(debug==1)
	{		
		cout<<"debug"<<index <<" "<<*addr<<endl;
	}
		string portString=(*addr).substr(index+1,(*addr).length()-1);
		std::string::size_type sz; 
		*port=stoi(portString,&sz);
		if(portString.substr(sz)!=""){
			if(debug==1)
			{		
				cout<<"ostalo: "<<portString.substr(sz)<<endl;
			}
			return false;
		}
		*addr=(*addr).substr(0,index);
	}
	else
	{
		*port=PORT_NUM;
	}
	return true;
}
bool isBeginning(string String,string substring)
{
	for(int i=0;i<substring.length()&&i<String.length();i++)
	{
		if(tolower(String[i]) != tolower(substring[i]))
		{
			return false;
		}
	}
	return true;
}

int getCode(string buf,int *type,string *location,bool *chunked)
{
// eva ~/IPK> ./webclient https://www.google.com
// pocet je: 2
// Adresa:https://www.google.com
// addr:www.google.com
// cesta:/

// HTTP/1.1 302 Found
// Cache-Control: private
// Content-Type: text/html; charset=UTF-8
// Location: http://www.google.cz/?gfe_rd=cr&ei=oeDqVonDO6eo8we6p4CIDQ
// Content-Length: 258
// Date: Thu, 17 Mar 2016 16:51:45 GMT
// Connection: close

	int code=0;
	string s="";string sddd;
	string codeS;
	int index=INT_MAX,index1=INT_MAX;
	*location=buf;
	//transform((*location).begin(), (*location).end(), (*location).begin(), tolower);
	if((index = (*location).find("Location:"))!= std::string::npos || (index1=(*location).find("location:"))!= std::string::npos)
	{
		if(index==std::string::npos)
			index=INT_MAX;
		if(index1==std::string::npos)
			index1=INT_MAX;
		if(index1<index){
			index=index1;
		}
		if(debug==1)
		{
			cout << *location<< endl;
		}
		(*location).erase(0, index+10);
		index = (*location).find("\n");
		(*location).erase(index);
	}
	if(debug==1)
	{
		cout<<"debug"<<endl;
	}
	for(int i=0;;i++)
	{
		//buf = buf.substr(s.length(), buf.length()-s.length());
		s = buf.substr(0, buf.find('\n'));
		//s=s.erase(s.length()-1,s.length()-1);
		buf=buf.erase(0,buf.find("\r\n")+2);
		//cout<<"debug "<<s<<endl;
		switch(i)
		{
			case 0:
				
				sddd=s.substr(0,8);
				//cout <<"csdvfbvdfbbfgbfgbfgbgfbggbgfbf "<< sddd<<endl;
				if(sddd=="HTTP/1.1")
					*type=1;
				else
					*type=0;
				codeS=s.substr(9,3);
				code=stringToint(codeS);
				if(debug==1)
				{
					cout << "code: " << code<<" "<<codeS<< endl;
				}
				if(code==200)
				{
					if(buf.find("Transfer-Encoding: chunked\r\n") != string::npos)
						*chunked=true;
					else
						*chunked=false;
				}
			return code;
			break;
			
			default:
			break;
		}
	}
	
	return code;
}

char* message(string addres, string cesta,int http,bool head)
{
	//"HEAD / HTTP/1.1\r\nHost: www.google.com\r\nConnection: close\r\n\r\n";
	string d="";
	if(head==1)
		d.append("HEAD ");
	else
		d.append("GET ");
	d.append(stringToCharArray(cesta));
	d.append(" HTTP/1.");
	d.append(intToCharArray(http));
	d.append("\r\nHost: ");
	d.append(stringToCharArray(addres));
	if(head==0)
		d.append("\r\nConnection: close\r\n\r\n");
	else
	if(http)
		d.append("\r\n\r\n");
	else
		d.append("\r\nConnection: keep-active\r\n\r\n");
		
	if(debug==1)
	{		
		cout<<endl<<endl<<d<<endl<<endl;
	}
	//ss>>d;
	return stringToCharArray(d);
}

string getFileName(string cestaKsuboru)
{
	
	string menoSuboru=cestaKsuboru.substr(cestaKsuboru.find(":") + 1); 
	menoSuboru.erase(0, 1);
	if(menoSuboru.find("/")!=std::string::npos)
		menoSuboru=menoSuboru.erase(0,menoSuboru.find_last_of("/")+1);
	if(menoSuboru=="")
	{
		return "index.html";
	}
	else
	{
		string s = "%20";
		string t = " ";
		int n = 0;
		//nahradenie pseudo medzier za medzery
		while ( ( n = menoSuboru.find( s, n ) ) != std::string::npos )
		{
			menoSuboru.replace( n, s.size(), t );
			n += t.size();
		}
		return menoSuboru;
	}
}

char* intToCharArray(int s)
{
	char *ss;
	sprintf(ss, "%d", s);
	return ss;
}
char* stringToCharArray(string s)
{
	return strdup(s.c_str());
}
int stringToint(string s)
{
	return atoi(s.c_str());
}














