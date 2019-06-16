class Header{
	public:
	char  ID[2];
	/*A 16 bit identifier assigned by the program that
		generates any kind of query.  This identifier is copied
		the corresponding reply and can be used by the requester
		to match up replies to outstanding queries.*/
	bool  QR;
	/*A one bit field that specifies whether this message is a
		query (0), or a response (1).*/
	bool Opcode [4];
	/*A four bit field that specifies kind of query in this
		message.  This value is set by the originator of a query
		and copied into the response.  The values are:

		0               a standard query (QUERY)

		1               an inverse query (IQUERY)

		2               a server status request (STATUS)

		3-15            reserved for future use*/
	bool AA;
	/*Authoritative Answer - this bit is valid in responses,
		and specifies that the responding name server is an
		authority for the domain name in question section.

		Note that the contents of the answer section may have
		multiple owner names because of aliases.  The AA bit
		corresponds to the name which matches the query name, or
		the first owner name in the answer section.*/
	bool TC;
	/*TrunCation - specifies that this message was truncated
		due to length greater than that permitted on the
		transmission channel.*/
	bool RD;
	/*Recursion Desired - this bit may be set in a query and
		is copied into the response.  If RD is set, it directs
		the name server to pursue the query recursively.
		Recursive query support is optional.*/
	bool RA;
	/*Recursion Available - this be is set or cleared in a
		response, and denotes whether recursive query support is
		available in the name server.*/
	bool Z [3];
	//Reserved for future use.  Must be zero in all queries and responses.
	bool RCODE [4];
	/*Response code - this 4 bit field is set as part of
                responses.  The values have the following
                interpretation:

                0               No error condition

                1               Format error - The name server was
                                unable to interpret the query.

                2               Server failure - The name server was
                                unable to process this query due to a
                                problem with the name server.

                3               Name Error - Meaningful only for
                                responses from an authoritative name
                                server, this code signifies that the
                                domain name referenced in the query does
                                not exist.

                4               Not Implemented - The name server does
                                not support the requested kind of query.

                5               Refused - The name server refuses to
                                perform the specified operation for
                                policy reasons.  For example, a name
                                server may not wish to provide the
                                information to the particular requester,
                                or a name server may not wish to perform
                                a particular operation (e.g., zone
                                	                                transfer) for particular data.

                6-15            Reserved for future use.*/
	uint16_t QDCOUNT;
	/*an unsigned 16 bit integer specifying the number of
		entries in the question section.*/
	uint16_t ANCOUNT;
	/*an unsigned 16 bit integer specifying the number of
		resource records in the answer section.*/
	uint16_t NSCOUNT;
	/*an unsigned 16 bit integer specifying the number of name
		server resource records in the authority records
		section.*/
	uint16_t ARCOUNT;
	/*an unsigned 16 bit integer specifying the number of
		resource records in the additional records section.*/
	void  toString(char *s);
};

void  Header::toString(char *s){//s musi byt 12 prvkove pole
	//char s[12];
	char byte1=((int)this->QR*128+(int)this->Opcode[0]*64+(int)this->Opcode[1]*32+(int)this->Opcode[2]*16+(int)this->Opcode[3]*8+(int)this->AA*4+(int)this->TC*2+ (int)this->RD*1);
	char byte2=((int)this->RA*128+(int)this->Z[0]*64+(int)this->Z[1]*32+(int)this->Z[2]*16+(int)this->RCODE[0]*8+(int)this->RCODE[1]*4+(int)this->RCODE[2]*2+ (int)this->RCODE[3]*1);
	
	char b3=(this->QDCOUNT>> 8) &0xFF;
	char b4=this->QDCOUNT &0xFF;
	/*>255?this->QDCOUNT:("0"+this->QDCOUNT)*/
	char b5=(this->ANCOUNT>> 8) &0xFF;
	char b6=this->ANCOUNT &0xFF;
	/*>255?this->ANCOUNT:("0"+this->ANCOUNT)*/
	char b7=(this->NSCOUNT>> 8) &0xFF;
	char b8=this->NSCOUNT &0xFF;
	/*>255?this->NSCOUNT:("0"+this->NSCOUNT)*/
	char b9=(this->ARCOUNT>> 8) &0xFF;
	char b10=this->ARCOUNT &0xFF;
	/*>255?this->ARCOUNT:("0"+this->ARCOUNT)*/
	//s=s+this->ID[0]+this->ID[1]+byte1+byte2+
	//b3+b4+b5+b6+b7+b8+b9+b10;

	s[0]=this->ID[0];
	s[1]=this->ID[1];
	s[2]=byte1;
	s[3]=byte2;
	s[4]=b3;
	s[5]=b4;
	s[6]=b5;
	s[7]=b6;
	s[8]=b7;
	s[9]=b8;
	s[10]=b9;
	s[11]=b10;
//printStringInBiteForm(s,12,"header");
	//return s;
}