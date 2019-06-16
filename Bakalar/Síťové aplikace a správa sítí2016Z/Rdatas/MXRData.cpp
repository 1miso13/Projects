#include <string>
class MX_RData :public RData
{
public:
	MX_RData(string exchange,string preference);
	~MX_RData();
	const char* getRData();
	string RetStringOUT();
	string exchange;
	string exchangeI;
	int16_t preference;
};
MX_RData::MX_RData(string exchange,string preference){
	this->exchange=exchange;
	this->exchangeI=convertToInternetForm(exchange);
	this->preference=atoi(preference.c_str());	
	length=exchangeI.length()+2;


	write16BitNum(data,0,this->preference);
	memcpy(data+2,exchangeI.c_str(),exchangeI.length());

}
const char* MX_RData::getRData(){
	return data;
}
string MX_RData::RetStringOUT(){
	return "preference " + to_string(preference) + ", mx " + exchange.substr(0, exchange.size()-1);
}