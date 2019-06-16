class NS_RData :public RData
{
public:
	NS_RData(string target);
	~NS_RData();
	string target;

	string targetI;
	const char* getRData();
	string RetStringOUT();
};
NS_RData::NS_RData(string target){
	this->target=target;

	targetI=convertToInternetForm(target);

	length=targetI.length();
	memcpy(data,targetI.c_str(),targetI.length());
}
const char* NS_RData::getRData(){
	return data;
}
string NS_RData::RetStringOUT(){
	return "ns " + target.substr(0, target.size()-1);
}