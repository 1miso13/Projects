#include "../pomocneSubory/another.h"
class RData
{
public:
	~RData();
	int length;//dlzka dat
	int32_t TTL;
	char *data = new char[512];//data
	virtual const char* getRData()=0;//metoda vrati data,ktore sa poslu
	virtual string RetStringOUT()=0;//metoda vrati retazec ktory sa vypise na stdout
};
RData::~RData()
{
	delete [] data;
}