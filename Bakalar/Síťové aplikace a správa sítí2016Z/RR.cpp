class RR
{
public:
	RR(char *Question, int questionLength,RData* Rdata);
	char *Question;
	int questionLength;
	int32_t TTL;
	uint16_t RDLength;
	RData *Rdata;
	int getRR(char * rr);
};
RR::RR(char *Question, int questionLength,RData* Rdata){
	this->Question=Question;
	this->questionLength=questionLength;
	this->TTL=Rdata->TTL;
	this->RDLength=Rdata->length;
	this->Rdata=Rdata;
}
int RR::getRR(char * rr){

	int offset=0;
	memcpy(rr,Question,questionLength);
	offset+=questionLength;

	write32BitNum(rr,offset,this->TTL);
	offset+=4;

	write16BitNum(rr,offset,this->RDLength);
	offset+=2;
	memcpy(rr+offset,Rdata->getRData(),Rdata->length);
	return questionLength/*name,type,class*/+4/*TTL*/+2/*RDLength*/+RDLength/*RDATA*/;

}
