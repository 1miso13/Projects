class CNAME_RData :public RData
{
public:
	CNAME_RData(string target);
	~CNAME_RData();
	string target;
	string targetI;
	const char* getRData();
	string RetStringOUT();
};
CNAME_RData::CNAME_RData(string target){
	this->target=target;
	this->targetI=convertToInternetForm(target);

	length=this->targetI.length();
	memcpy(data,this->targetI.c_str(),this->targetI.length());
}
const char* CNAME_RData::getRData(){
	return data;
}
string CNAME_RData::RetStringOUT(){
	return "cname "+target.substr(0, target.size()-1);
}