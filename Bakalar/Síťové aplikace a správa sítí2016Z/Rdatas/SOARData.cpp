class SOA_RData :public RData
{
public:
	SOA_RData(string mname,string rname,string serial,string refresh,string retry,string expire,string minimum);
	~SOA_RData();
	const char* getRData();
	string RetStringOUT();
	string mname;
	string rname;
	string mnameI;
	string rnameI;
	uint32_t serial;
	uint32_t refresh;
	uint32_t retry;
	uint32_t expire;
	uint32_t minimum;
};
SOA_RData::SOA_RData(string mname,string rname,string serial,string refresh,string retry,string expire,string minimum){
	this->mname=mname;
	this->mnameI=convertToInternetForm(mname);
	this->rname=rname;	
	this->rnameI=convertToInternetForm(rname);
	this->serial=atoi(serial.c_str());
	this->refresh=atoi(refresh.c_str());
	this->retry=atoi(retry.c_str());
	this->expire=atoi(expire.c_str());
	this->minimum=atoi(minimum.c_str());
	length=mnameI.length()+rnameI.length()+(4*5);
	

	int offset=0;
	memcpy(data,mnameI.c_str(),mnameI.length());
	offset+=mnameI.length();
	memcpy(data+offset,rnameI.c_str(),rnameI.length());
	offset+=rnameI.length();
	write32BitNum(data,offset,this->serial);
	offset+=4;
	write32BitNum(data,offset,this->refresh);
	offset+=4;
	write32BitNum(data,offset,this->retry);
	offset+=4;
	write32BitNum(data,offset,this->expire);
	offset+=4;
	write32BitNum(data,offset,this->minimum);

}
const char* SOA_RData::getRData(){
	return data;
}
string SOA_RData::RetStringOUT(){
	return "mname "+mname.substr(0, mname.size()-1);
}