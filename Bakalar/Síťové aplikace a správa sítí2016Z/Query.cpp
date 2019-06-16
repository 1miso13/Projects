#include <limits.h>
#include <vector>
#include <algorithm>
#include "Header.cpp"
#include "Rdatas/Rdata.h"
#include <cstddef>
#include "RR.cpp"
#include <sstream>
#include <string>
string getString(FILE * in);
void addToCharArray(char buffer[],char *message,int start,int length);
/*

*/
class Query{
private:
	Header *header;
//telo:
	char originalQuestion[512];
	int originalQuestionLength;
	char QNAME[512];
	/*a domain name represented as a sequence of labels, where
		each label consists of a length octet followed by that
		number of octets.  The domain name terminates with the
		zero length octet for the null label of the root.  Note
		that this field may be an odd number of octets; no
		padding is used.*/
	char QTYPE[2];
	/*a two octet code which specifies the type of the query.
		The values for this field include all codes valid for a
		TYPE field, together with some more general codes which
		can match more than one type of RR.*/
	char QCLASS[2];
	/*a two octet code that specifies the class of the query.
		For example, the QCLASS field is IN for the Internet.*/
	bool valid;
public:
	Query(char* str,int length);
	bool testValid();
	void print();
	string createQuestionOUT();
	void createAnswer(vector<string> zonefiles,char *answer,int * answerLength,string MitmIP);
	string createName();
	string selectType(int type);
	~Query();
	Header *createResponseHeader(bool autoritative,bool TC,unsigned int rcode,uint16_t qdCount ,uint16_t anCount,uint16_t nsCount,uint16_t arCount);
	void recursiveCall(vector<RData *>* rdatas);
};
Query::~Query(){
	delete header;
}
/*konstruktor*/
Query::Query(char* str,int length){

	valid=true;
	if(length<18){
		valid=false;
		return;
	}
	header=new Header();
//rozparsujeme prvych 12B na Hlavicku
	header->ID[0]=str[0];//1.
	header->ID[1]=str[1];//2.

	//2B treba rozdelit na bity 
//3.
	char a=str[2];
	header->QR= a & (1 << 7);
	header->Opcode [0]=a & (1 << 6);
	header->Opcode [1]=a & (1 << 5);
	header->Opcode [2]=a & (1 << 4);
	header->Opcode [3]=a & (1 << 3);
	header->AA=a & (1 << 2);
	header->TC=a & (1 << 1);
	header->RD=a & (1 << 0);
//4.
	a=str[3];
	header->RA=a & (1 << 7);
	header->Z[0]=a & (1 << 6);
	header->Z[1]=a & (1 << 5);
	header->Z[2]=a & (1 << 4);
	header->RCODE[0]=a & (1 << 3);
	header->RCODE[1]=a & (1 << 2);
	header->RCODE[2]=a & (1 << 1);
	header->RCODE[3]=a & (1 << 0);

	header->QDCOUNT=str[4]*4 + (str[5] >> (CHAR_BIT - 2));//5.
	header->ANCOUNT=str[6]*4 + (str[7] >> (CHAR_BIT - 2));//6.
	header->NSCOUNT=str[8]*4 + (str[9] >> (CHAR_BIT - 2));//7.
	header->ARCOUNT=str[10]*4 + (str[11] >> (CHAR_BIT - 2));//8.
	

	int i = 12;
	char q[512];
	int QnameLength=0;
	//zistime si kolko bajtov zabera QNAME a ulozime si do QNAME
	for (int j=0;  buffer[i-1]!=0; ++i,j++)
	{
		if (i >= length-4)//ak presiahlo dlzku tak sa hodi chyba
		{
			valid=false;
			break;
		}
		else
		{
			QNAME[j] = buffer[i];
			
			q[j]=QNAME[j];
			QnameLength=j+1;
		}
	}


	QTYPE[0] = buffer[i];
	QTYPE[1] = buffer[i+1];
	QCLASS[0] = buffer[i+2];
	QCLASS[1] = buffer[i+3];
	i+=4;
	//testovaci vypis
	/*cout<<"TEST " << i <<endl;
	for (int j=0; i < length; ++i,j++)
	{
		cout<< i << " "<< j<< ".   " << (int)buffer[i] << "   "<< buffer[i]<<endl ;
		//this.QTYPE[j] = buffer[i];
	}*/

	memcpy(q,QNAME,QnameLength);
	//cout << QnameLength<<endl;
	q[QnameLength]=QTYPE[0];
	q[QnameLength+1]=QTYPE[1];
	q[QnameLength+2]=QCLASS[0];
	q[QnameLength+3]=QCLASS[1];

	originalQuestionLength=QnameLength+4;
	memcpy(originalQuestion,q,originalQuestionLength);
}
bool Query::testValid(){
	if (this->valid==true)
	{
	    if(header->QR!=0 || QCLASS[0]!=0 || QCLASS[1]!=1){
	    	this->valid=false;
	    }
	}
	if (this->valid==false)
	{
		originalQuestion[0]='\0';
		originalQuestionLength=0;
	}
	return this->valid;
}
/**
 *   vypise na stdout prijate data
 **/
void Query::print(){
	/*char  ID[2];
	bool  QR;
	bool Opcode [4];
	bool AA;
	bool TC;
	bool RD;
	bool RA;
	bool Z [3];
	bool RCODE [4];
	unsigned int QDCOUNT;
	unsigned int ANCOUNT;
	unsigned int NSCOUNT;
	unsigned int ARCOUNT;
	char QNAME[512];
	char QTYPE[2];
	char QCLASS[2];*/

	cout<< "ID:" << header->ID[0] << header->ID[1] <<endl;
	cout << "QR:"<< header->QR<< endl;
	cout << "Opcode [4]:"<< (int)header->Opcode[0]<< (int)header->Opcode[1]<< (int)header->Opcode[2]<< (int)header->Opcode[3]<<endl;
	cout << "AA:"<< header->AA << endl;
	cout << "TC:"<< header->TC << endl;
	cout << "RD:"<< header->RD << endl;
	cout << "RA:"<< header->RA << endl;
	cout << "Z [3]:"<< (int)header->Z[0]<< (int)header->Z[1]<< (int)header->Z[2] << endl;
	cout << "RCODE [4]:"<< (int)header->RCODE[0]<< (int)header->RCODE[1]<< (int)header->RCODE[2]<< (int)header->RCODE[3] << endl;
	cout << "QDCOUNT:"<< header->QDCOUNT << endl;
	cout << "ANCOUNT:"<< header->ANCOUNT << endl;
	cout << "NSCOUNT:"<< header->NSCOUNT << endl;
	cout << "ARCOUNT:"<< header->ARCOUNT << endl;
	cout << "QNAME[512]:"<< QNAME << endl;
	cout << "QTYPE[2]:"<< (int)QTYPE[0] << (int)QTYPE[1] << endl;
	cout << "QCLASS[2]:"<< (int)QCLASS[0] << (int)QCLASS[1] << endl;
}
//text ktory sa vypise na stdout
string Query::createQuestionOUT(){
	string addr = createName();
	string TYPE=selectType(QTYPE[1]);
	if (QTYPE[0]!=0||selectType(QTYPE[1]).compare("NOT_IMPLEMENTED") == 0)
	{
		int d=QTYPE[0]*256+QTYPE[1];
		TYPE="Unknown ("+to_string(d)+")";
	}
	return addr+": type "+TYPE+", class IN";
}
string Query::selectType(int type){
	string TYPE="";
	switch(QTYPE[1]){
		case 1:TYPE="A";
		break;
		case 2:TYPE="NS";
		break;
		case 5:TYPE="CNAME";
		break;
		case 6:TYPE="SOA";
		break;
		case 12:TYPE="PTR";
		break;
		case 15:TYPE="MX";
		break;
		case 16:TYPE="TXT";
		break;
		case 28:TYPE="AAAA";
		break;
		default:
			TYPE="NOT_IMPLEMENTED";
		//chyba
		break;
	}
	return TYPE;
}
void Query::createAnswer(vector<string> zonefiles,char *answer,int * answerLength,string MitmIP){
	//vytvory response header
	string RetString ;
	bool FoundInZoneFile=false;
	vector<RData *> rdatas;
	unsigned int rcode=0;
	bool autoritative=false;

	if (!testValid())
	{
		rcode=1;
	}
	else
	{
		if (QTYPE[0]!=0||selectType(QTYPE[1]).compare("NOT_IMPLEMENTED") == 0)
		{
			rcode=4;
		}
		else
		{
			if (MitmIP.compare("")!=0 && (selectType(QTYPE[1]).compare("A") == 0 || selectType(QTYPE[1]).compare("MX") == 0))
			{//ak bolo zadane mitm
				autoritative=true;
				RData *rdata;
				if (selectType(QTYPE[1]).compare("A") == 0)
				{
					rdata=new A_RData(MitmIP);
				}
				else{
					int EXCHANGE_COUNT=16;
					char exchangeCHA[EXCHANGE_COUNT];
					bool NF=true;
					do
					{
	    				srand(time(NULL));
						gen_random(exchangeCHA,EXCHANGE_COUNT);

						exchangeCHA[(rand() % (EXCHANGE_COUNT - 3))+1]='.';//na nahodne cislo sa umiesni . a nesmie to byt prvy ani posledny znak
						//zistime ci sa nenachadza v zonefile
						for (int i = 0; i < zonefiles.size(); ++i)
						{
							string zonefile=zonefiles[i];
							string str(exchangeCHA);
							str+=".";//na konci musi byt bodka
							string param= ("./roughDNS.py " + zonefile+" "+ str +" MX");
							
							FILE* in = popen(param.c_str(), "r");
							RetString =getString(in);
							if (RetString.compare("ERROR")==0)
							{
								cerr << "Nastala chyba v zonefile"  << endl;
								pclose(in);
								answer[0]='\0';
								return; 
							}
							if (RetString.compare("NOT_FOUND")==0)
							{
								//nasli sme 
								NF=false;
							}
						}
					} while (NF);

					string exchange(exchangeCHA);
					string preference="1";
					rdata=new MX_RData(exchange+".",preference);
				}
				rdata->TTL=3600;
				

				rdatas.push_back(rdata);
			}
			else
			{

				for(int i=0;i<zonefiles.size();i++){
					string zonefile=zonefiles[i];
					string param= ("./roughDNS.py " + zonefile +" "+createName() +" "+selectType(QTYPE[1]));
					//testovaci vystup
					//cout<< "param:" <<param <<endl;
					FILE* in = popen(param.c_str(), "r");

					RetString =getString(in);
					
					if (RetString.compare("ERROR")==0)
					{
						
						cerr << "Nastala chyba v zonefile"  << endl;

						pclose(in);
						answer[0]='\0';
						return; 
					}
					if (RetString.compare("NOT_FOUND")==0)
					{
						continue;
					}
					//nasli sme zaznam, opakujeme pokym bude nieco na vstupe
					autoritative=true;
					while (RetString.compare("OK")==0)
					{
						RData *rdata;
						switch(QTYPE[1]){
							case 1://A
							{
								string address=getString(in);
								rdata=new A_RData(address);
							}
							break;
							case 2://"NS";
							{
								string target=getString(in);
								rdata=new NS_RData(target);
							}
							break;
							case 5://"CNAME";
							{
								string target=getString(in);
								rdata=new CNAME_RData(target);
							}
							break;
							case 6://"SOA";
							{
								string mname=getString(in);
								string rname=getString(in);
								string serial=getString(in);
								string refresh=getString(in);
								string retry=getString(in);
								string expire=getString(in);
								string minimum=getString(in);
								rdata=new SOA_RData(mname,rname,serial,refresh,retry,expire,minimum);
							}
							break;
							case 12://"PTR";
							{
								string target=getString(in);
								//rdata=new PTR_RData(target);
							}
							break;
							case 15://"MX";
							{
								string exchange=getString(in);
								string preference=getString(in);
								rdata=new MX_RData(exchange,preference);
							}
							break;
							case 16://"TXT";
							{
								int count=0;
								stringToint(getString(in),&count);
								string txtString="";

									for (int i = 0; i < count; ++i)
									{
										string str=getString(in);
										txtString+=(i==0?"":"\" , \"")+str;
									}
								rdata=new TXT_RData(txtString);
							}
							break;
							case 28://"AAAA";
							{
								string address=getString(in);
								rdata=new AAAA_RData(address);
							}
							break;
							default:
							{
								
							//chyba
							}
							break;
						}
						RetString=getString(in);
						stringstream ss;
						ss<<RetString;
						ss>>rdata->TTL;
						FoundInZoneFile=true;

						rdatas.push_back(rdata);

						RetString=getString(in);
					}


					pclose(in);
				}
				if (FoundInZoneFile==false)
				{
					//musime hladat inde alebo vratit ze nevieme odpovedat
					//zavolame dig na iny dns server
					recursiveCall(&rdatas);
					if (rdatas.size()!=0)
					{
						//nasli sme zaznam
						autoritative=false;

					}
					else
					{

						//ak sa nenaslo tak nastavime rcode
						rcode=3;
						autoritative=true;
					}


				}
			}
		}
	}
	//bool TC,
	uint16_t qdCount=1;
	uint16_t anCount=rdatas.size();
	//unsigned int16_t nsCount,
	//unsigned int16_t arCount
	/*for (int i = 0; i < 12; ++i)
	{
		cout << i <<"reply:";
		for (int j = 0; j < 8; ++j)
		{
			cout<<!!((s[i] << j) & 0x80);
		}
		cout<< endl;
	}*/
	int rrsLength=0;
	char answerBody[500];//=512-12(velkost hlavicky)
	int TEMPanswerLength=0;
	//vypiseme na STDOUT
	for (int i = 0; i < rdatas.size(); ++i)
	{
		cout << "r: " << createQuestionOUT()+", "+rdatas[i]->RetStringOUT()<<endl;
		
		RR *rr = new RR(originalQuestion,originalQuestionLength,rdatas[i]);
		char rrBody[500];
		int rrlength=rr->getRR(rrBody);

		rrsLength +=rrlength;
		memcpy(answerBody+TEMPanswerLength,rrBody,rrlength);
		TEMPanswerLength+=rrlength;
		delete rr;
	}
	//vymazanie rdata
	for (int i = rdatas.size()-1; i >=0; i--)
	{
		delete rdatas[i];
	}
	int headerLength=12;
	//nastavime velkost dat ktore budu odoslane
	*answerLength=headerLength/*header*/+originalQuestionLength/*question*/+rrsLength/*answer*/;

	//vratime 
	if (*answerLength>512)
	{
		cerr << "Truncated nepodporovane";
		answer[0]='\0';
		return;
	}
	char stringHeader[headerLength];
	stringHeader[0]='\0';
	Header *responseHeader =createResponseHeader(autoritative,false,rcode,qdCount,anCount,0,0);
	
	responseHeader->toString(stringHeader);

	delete responseHeader;
	
	//printStringInBiteForm(originalQuestion,originalQuestionLength,"Q:");
addToCharArray(answer,stringHeader,0,headerLength);
addToCharArray(answer,originalQuestion,headerLength,originalQuestionLength);
	//printStringInBiteForm(answerBody,rrsLength,"Answer:");
addToCharArray(answer,answerBody,headerLength+originalQuestionLength,rrsLength);
	//return buffer;//stringHeader+originalQuestion+answerBody;

	
}
//vlozi data do buffera
void addToCharArray(char buffer[],char *message,int start,int length){
	for (int i = 0; i < length; ++i)
	{
		buffer[start+i]=message[i];
	}
}
//postupne berie data z File*
string getString(FILE * in){
	char RetString1 [100];
	fgets (RetString1 , 100 , in);
	RetString1[strcspn(RetString1, "\n")] = 0;
	return (RetString1);
}
//zmeni z internetoveho tvaru na normalny tvar 
string Query::createName(){
	string name="";
	int j;
	bool d=false;
	for (int i = 0; (j=QNAME[i])!=0; )
	{
		//cout<<endl<< j <<endl;
		name+=d?".":"";
		d=true;
		i++;
		for (; j >0; j--,i++)
		{
			name+=QNAME[i];
		}
		
	}
	return name;
}
//vytvori hlavicku pre odpoved
Header *Query::createResponseHeader(bool autoritative,bool TC,unsigned int rcode,uint16_t qdCount ,uint16_t anCount,uint16_t nsCount,uint16_t arCount){
	Header *responseHeader= new Header();

	responseHeader->ID[0]=header->ID[0];
	responseHeader->ID[1]=header->ID[1];

	responseHeader->QR=1;

	responseHeader->Opcode[0]=header->Opcode[0];
	responseHeader->Opcode[1]=header->Opcode[1];
	responseHeader->Opcode[2]=header->Opcode[2];
	responseHeader->Opcode[3]=header->Opcode[3];

	//treba zistit ci je autoritativny
	responseHeader->AA=autoritative;

	//truncation - mame podporovat iba UDP, preto by malo byt vzdy 0
	responseHeader->TC=TC;

	responseHeader->RD=header->RD;

	// server ma  podporovat rekurziu
	responseHeader->RA=1;


	responseHeader->Z[0]=0;
	responseHeader->Z[1]=0;
	responseHeader->Z[2]=0;


	//rcode 0-15

	responseHeader->RCODE[3]=rcode%2;
	rcode/=2;
	responseHeader->RCODE[2]=rcode%2;
	rcode/=2;
	responseHeader->RCODE[1]=rcode%2;
	rcode/=2;
	responseHeader->RCODE[0]=rcode%2;

	responseHeader->QDCOUNT=qdCount;
	responseHeader->ANCOUNT=anCount;
	responseHeader->NSCOUNT=nsCount;
	responseHeader->ARCOUNT=arCount;
	return responseHeader;
}
//posleme dig na defaultny DNS server rekurzivne a prijmeme odpoved
void Query::recursiveCall(vector<RData *>* rdatas){

	string call = "dig +nocmd +noall +answer +ttlid "+ selectType(QTYPE[1]) + " " + createName();
	FILE* in = popen(call.c_str(), "r");
	int size=512;
	char RetString[size];
	char RetStringOLD[size];
	RetString[0]='\0';
	//zoberieme prve aby sme zitili ci sa naslo aspon jedno
	fgets (RetString , size , in);
	if (strcmp (RetString,"") != 0)
	{
		do
		{

			//musime rozparsovat 
			vector<string> split;
			string str(RetString);
			str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
			replace( str.begin(), str.end(), ' ', '\t');
			stringstream ss(str); // Turn the string into a stream.
			string tok;
			char delimiter='\t';
			while(getline(ss, tok, delimiter)) {
				if (tok.compare("")!=0 && tok.compare("\n")!=0)
				{
					split.push_back(tok);
				}
			}
			//test ci sme dostali spravny typ zaznamu
			if (split[3].compare(selectType(QTYPE[1]))==0)
			{

				//testovaci vypis
				/*for(int i = 0; i < split.size(); ++i){
					cout << split[i] << endl;
				}*/

				//mozeme zacat tvorit rdata
				string ttl;
				RData *rdata;
				bool badType=false;
				switch(QTYPE[1]){
					case 1://A
					{
						string address=split[4];
						rdata=new A_RData(address);
						ttl=split[1];
					}
					break;
					case 2://"NS";
					{
						string target=split[4];
						rdata=new NS_RData(target);
						ttl=split[1];
					}
					break;
					case 5://"CNAME";
					{
						string target=split[4];
						rdata=new CNAME_RData(target);
						ttl=split[1];
					}
					break;
					case 6://"SOA";
					{
						string mname=split[4];
						string rname=split[5];
						string serial=split[6];
						string refresh=split[7];
						string retry=split[8];
						string expire=split[9];
						string minimum=split[10];
						rdata=new SOA_RData(mname,rname,serial,refresh,retry,expire,minimum);
						ttl=split[1];
					}
					break;
					case 12://"PTR";
					{
						string target=getString(in);
						//rdata=new PTR_RData(target);
					}
					break;
					case 15://"MX";
					{
						string exchange=split[5];
						string preference=split[4];
						rdata=new MX_RData(exchange,preference);
						ttl=split[1];
					}
					break;
					case 16://"TXT";
					{

						string ttl=split[1];
						//zavolame znovu dig ale tentokrat s parametrom +short aby sme dostali iba samotnu spravu (mohli by sa objavit divne znaky)
						//dig +short TXT google.sk
						string call = "dig +short "+ selectType(QTYPE[1]) + " " + createName();
						FILE* in2 = popen(call.c_str(), "r");

						fgets (RetString , size , in2);
						string txtString(RetString);
						pclose(in2);
						//if (txtString.front()=='\"' && txtString.back()=='\"')
						{
							txtString = txtString.substr(1, txtString.length()-3);
						}
						//ostranime prvy a posledny kedze su uvodzovky

						rdata=new TXT_RData(txtString);
						ttl=split[1];
					}
					break;
					case 28://"AAAA";
					{
						string address=split[4];
						rdata=new AAAA_RData(address);
						ttl=split[1];
					}
					break;
					default:
					{
						badType=true;
					//chyba
					}
					break;
				}
				stringstream ss1;
				ss1<<RetString;
				if (badType==false)
				{
					ss1>>rdata->TTL;
					//ulozime data do vektora
					(*rdatas).push_back(rdata);
				}
			}
			//zobereme dalsie a porovname ci nieje rovnake
			memcpy(RetString,RetStringOLD,size); 
			fgets (RetString , size , in);
			//cout<<RetString;
		}while(strcmp (RetString,RetStringOLD) != 0);
	}
	pclose(in);
}