class AAAA_RData :public RData
{
public:
	AAAA_RData(string address);
	~AAAA_RData();
	string address;
	const char* getRData();
	char buffer [16];
	string RetStringOUT();
};
AAAA_RData::AAAA_RData(string address){
	this->address=address;
   	inet_pton(AF_INET6, address.c_str(), buffer);
	length=16;

	for (int i = 0; i < 16; ++i)
	{
		data[i]=buffer[i];
	}
}
const char* AAAA_RData::getRData(){
	return data;
}
string AAAA_RData::RetStringOUT(){
	return "addr " + address;
}