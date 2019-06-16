class A_RData :public RData
{
public:
	A_RData(string address);
	~A_RData();
	string address;
	const char* getRData();
	char buffer [4];
	string RetStringOUT();
};
A_RData::A_RData(string address){
	this->address=address;
   	inet_pton(AF_INET, address.c_str(), buffer);
	length=4;
	for (int i = 0; i < 4; ++i)
	{
		data[i]=buffer[i];
	}
}
const char* A_RData::getRData(){
	return data;
}
string A_RData::RetStringOUT(){
	return "addr " + address;
}