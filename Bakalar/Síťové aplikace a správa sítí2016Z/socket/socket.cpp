int setSocket(struct sockaddr_in *server_addr,int port){
	int s;
	if ((s=socket(AF_INET, SOCK_DGRAM, 0)) != -1)
	{
		memset((char *) &(*server_addr), 0, sizeof(*server_addr));
		(*server_addr).sin_family = AF_INET;
		(*server_addr).sin_port = htons(port);
		(*server_addr).sin_addr.s_addr = htonl(INADDR_ANY);
	}
	return s;
}