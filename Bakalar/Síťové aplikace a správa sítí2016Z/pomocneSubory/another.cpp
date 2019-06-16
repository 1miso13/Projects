bool stringToint(string s,int *value){
	char* p;
    strtol(s.c_str(), &p, 10);
    *value= atoi(s.c_str());
	return *p == 0;
}

string convertToInternetForm(string d){
    d='%'+d;
    char c;int k=0,j=0;
    for(int i=0;i<d.length();i++){
        c= d[i];
        if(d[i]=='.'){
            d[k]=j;
            j=0;
            k=i;
        }
        else{
            if(i>0){
               j++;
               d[i]=c;
            }
        }
       
    }
    if(d[d.length()-1]=='.'){
        d[d.length()-1]='\0';
    }
    else
    {
        d[k]=j;
        d=d+'\0';
       
    }
	
    return d;
}
void printStringInBiteForm(char *s,int length,string p="")
{
    if (p.compare("")!=0)
    {
        cout<<p<<endl;
    }
    for (int i = 0; i < length; ++i)
    {
        cout << i <<"reply:";
        for (int j = 0; j < 8; ++j)
        {
            cout<<!!((s[i] << j) & 0x80);
        }
        cout<< std::hex << " " << (uint)s[i] <<  std::dec<< endl;
    }
}

void write32BitNum(char *bytes,int frombyte,int32_t n){
    bytes[frombyte+0] = (n >> 24) & 0xFF;
    bytes[frombyte+1] = (n >> 16) & 0xFF;
    bytes[frombyte+2] = (n >> 8) & 0xFF;
    bytes[frombyte+3] = n & 0xFF;
}
void write16BitNum(char *bytes,int frombyte,int16_t n){
    bytes[frombyte+0] = (n >> 8) & 0xFF;
    bytes[frombyte+1] = n & 0xFF;
}
void write8BitNum(char *bytes,int frombyte,uint8_t n){
    bytes[frombyte+0] = n & 0xFF;
}
void gen_random(char *s, const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}