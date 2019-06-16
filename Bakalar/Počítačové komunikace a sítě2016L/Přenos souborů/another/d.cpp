
int send(int clientSocket,string msg){
	if(vystup)
		cout<<"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" <<msg.size()<<"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"  <<endl;
	
	return send(clientSocket, (msg+"\r\n\r\n").data(), (msg+"\r\n\r\n").size(), 0);
}
bool receiv(int clientSocket,string *str,int length=0,string ID=""){
	if(vystup)
		cout <<"1234567899876543211234569987654321"<<endl;
	char buffer[BUFSIZ];
	bool uspesnost=true;
	int fff;
	double g=0;
	double e=-1,eprev=-1;
	*str="";
	for(;;){
		memset(buffer, 0, BUFSIZ *sizeof (char));
		fff=recv(clientSocket, buffer, BUFSIZ, 0);
		if(fff<0){
			//chyba prenosu				
			cerr<<"Nastala chyba prenosu"<<endl;
			uspesnost=false;
			break;
		}
		(*str).append(buffer, fff);//+=buffer[i];
		// for(int i=0;i<fff;i++)
		// {
			// cout<<"wasd:"<<i<<" "<<(unsigned int)buffer[i]%256<<endl;
		// }
		g+=fff;
		eprev=e;
		e=(g/((double)(length+4)))*100;
		if(vystup)
			if(length!=0 && e!=eprev){
				cout <<((ID=="")?(""):(ID+">>"))/*<< g << "<<"*/<<e<< "%"<<endl;
			}
		// cout <<"Prijate "<<buffer<<endl;
		// if((*str)[(*str).length()-1]==16)
			// (*str).pop_back();
		if((*str).find("\r\n\r\n")!=-1)
		{
			if(vystup)
				cout <<"end of recv"<<endl;
			break;
		}
	}
	if(vystup)
		cout<<"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" << (*str).size()<<"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"  <<endl;
	
	//treba odstranit koniec spravy 
	(*str).pop_back();
	(*str).pop_back();
	(*str).pop_back();
	(*str).pop_back();
	return uspesnost;
}
int stringToint(string s)
{
	return atoi(s.c_str());
}

bool isBegining(string String,string SubString){
	if(String.length()<SubString.length())
		return false;
	for(int i=0;i<SubString.length();i++)
		if(String[i]!=SubString[i])
			return false;
	return true;
}

bool isEnding(string String,string SubString){
	if(String.length()<SubString.length())
		return false;
	int i=0;
	for(;i<SubString.length();i++){
		// cout<<"testENDA:"<<(unsigned int)String[String.length()-1-i]%256 <<endl;
		// cout<<"testENDB:"<<(unsigned int)SubString[SubString.length()-1-i]%256		<<endl;
		if(String[String.length()-1-i]!=SubString[SubString.length()-1-i])
			return false;
	}
	return true;
}

bool getFile(string fileName,string *data,int *length){
	streampos size;
	char * memblock;

	ifstream file (fileName, ios::in|ios::binary);
	if (file.is_open())
	{
		// size = file.tellg();
		// memblock = new char [size];
		// file.seekg (0, ios::beg);
		// cout << "reading file";
		// file.read (memblock, size);
		
		// *length=(int)size;
		// *data=(string)memblock;
		// delete[] memblock;
		stringstream buf;
		buf << file.rdbuf();
		*data=buf.str();
		*length=(*data).size();
		file.close();
	}
	else {
		cerr << "Unable to open file";
		return false;
	}

	// cout << (int)*data%256;
	return true;
}
bool saveFile(string fileName,string dataToSave){
	//cout << "saving data length:\n"<<dataToSave.length()<<endl;
	ofstream file;
	file.open(fileName.c_str(), ofstream::out | ofstream::binary);
	file << dataToSave;
	file.close();
	return true;
}


string getFileName(string str)
{
	
	if(str.find("/")!=std::string::npos)
		str=str.erase(0,str.find_last_of("/")+1);
	return str;
}



