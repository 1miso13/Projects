parameters::parameters(void){
    port=53;
    m=false;
    p=false;
}
int parametry(int argc,char*argv[],parameters *param){
	if(DUBUG==1){
		cout<< "Zaciatok spracovania.\nPocet parametrov: " << argc << endl;
	}
	param->ipAddress=""; //ak nieje zadane MITM
	int i=1;
	int retPort=0;
	if ((retPort=setPort(argc,argv,param,&i))!=0)
	{
		return retPort;
	} 
	if(i<argc &&( strcmp(argv[i],"-m")== 0 || strcmp(argv[i],"--mitm")== 0)){
		if(DUBUG==1){
			cout<< "MITM"<< endl;
		}
		if(argc>i+1){
			param->ipAddress=argv[i+1];
			//test ci je ip adresa validna
			char IP[4];
			if(inet_pton(AF_INET,param->ipAddress.c_str(),&IP)!=1)
			{
				cerr<< "IP ADDRESS - NOT VALID"<<endl;
				return ERR_MITM_ADDRESS;
			}
			param->m=true;
			i++;
			i++;
		}
		else{
			cerr<< "Zly pocet parametrov" << endl;
			return ERR_COUNT_OF_PARAM;
		}
	}
	if(i<argc &&(strcmp(argv[i],"-h")== 0 || strcmp(argv[i],"--help")== 0)){
		//help
		if(DUBUG==1){
			cout<< "HELP"<< endl;
		}
		if(argc==2){
			string help="DNS server - Michal Ondrejo\npriklady spustenia:\n"
"./roughDNS                              //server sa spusti na porte 53\n"
"./roughDNS -h                           //vypise na STDOUT napovedu k programu\n"
"./roughDNS --help                       //rovnake ako -h\n"
"./roughDNS -m <ip_address>              //resolvuje kazdy dotaz na A a MX na zvolenu IP\n"
"./roughDNS --mitm <ip_address>          //rovnake ako -m\n"
"./roughDNS -p <port>                    //server sa spusti na zadanom porte\n"
"./roughDNS --port <port>                //rovnake ako -p\n"
"./roughDNS -m <ip_address> -p <port>    //spusti sa server na zadanom porte a resolvuje vsetky dotazy na A a MX na zvolenu IP adresu\n"
"./roughDNS [<zonefile>]                 //server sa spusta na porte 53, v subore zonefile sú ulozené DNS záznamy. Ak sa daný záznam nenachádza v zonefile, server to vyriesi rekurzívne\n"
"./roughDNS [<zonefile>] [<zonefile1>] [<zonefileX>] //priklad s viac zonovými súbormi\n"
"./roughDNS -m <ip_address> -p <port> [<zonefile>]   //príklad spustenia servera s parametrami -m, -p a zonovým súborom";
			cout<< help <<endl;
			return HELP;
		}
		else{
			
			//s -h bol zadany este nejaky iny parameter
			cerr<< "Zly pocet parametrov" << endl;
			return ERR_HELP;
		}
	}
	if ((retPort=setPort(argc,argv,param,&i))!=0)
	 {
	 	return retPort;
	 } 
	/*if(i<argc &&(strcmp(argv[i],"-p")== 0 || strcmp(argv[i],"--port")== 0)){
		if(DUBUG==1){
			cout<< "PORT"<< endl;
		}
		if(argc>i+1){
			int port;
			if (stringToint(argv[i+1],&port))
			{
				
				param->port=port;
			}
			else{
				return ERR_ZLE_ZADANY_PORT;
			}
		}
		else{
			return ERR_COUNT_OF_PARAM;
		}
		
		//navysime o 2 pretoze mame -p a cislo portu
		i++;
		i++;
	}*/
	
	//vsetko ostatne su zonefile
	for (; i < argc; ++i)
	{
		ifstream my_file(argv[i]);
		if (!my_file.good())
		{
		  	cerr<<"Zonefile " << argv[i] <<" can't be found or can't read."<< endl;
			
			return ERR_ZONE_FILE;
		}
		param->zonefiles.push_back(argv[i]);
	}
	
	return VALID;
}
int setPort(int argc,char*argv[],parameters *param,int *i){
	if(*i<argc &&(strcmp(argv[*i],"-p")== 0 || strcmp(argv[*i],"--port")== 0)){
		param->p=true;
		if(argc>(*i)+1){
			int port;
			if (stringToint(argv[(*i)+1],&port))
			{
				param->port=port;
			}
			else{
				cerr<< "Zle zadany port" <<endl;
				return ERR_ZLE_ZADANY_PORT;
			}
		}
		else{
			cerr<< "Zly pocet parametrov" << endl;
			return ERR_COUNT_OF_PARAM;
		}
		//navysime o 2 pretoze mame -p a cislo portu
		(*i)++;
		(*i)++;
	}
	return 0;
}