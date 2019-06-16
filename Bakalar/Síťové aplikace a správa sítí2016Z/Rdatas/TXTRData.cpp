class TXT_RData : public RData
{
public:
	TXT_RData(string txtString);
	~TXT_RData();
	const char* getRData();
	string RetStringOUT();
	string txtString;
};
TXT_RData::TXT_RData(string txtString){
	this->txtString=txtString;
	length=txtString.length()+1;

	write8BitNum(data,0,(uint8_t)txtString.length());
	memcpy(data+1,txtString.c_str(),txtString.length());
}
const char* TXT_RData::getRData(){
	return data;
}
string TXT_RData::RetStringOUT(){
	return "txt \""+txtString +"\"";
}