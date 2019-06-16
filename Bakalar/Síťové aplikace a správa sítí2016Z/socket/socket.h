#include <sys/socket.h>
#include <arpa/inet.h>
#define ERR_SOCKET -100
#define ERR_SOCKET_BIND -101
#define BUFFER_LEN 512
using namespace std;
char buffer[BUFFER_LEN];
int setSocket(struct sockaddr_in *server_addr,int port);
#include "socket.cpp"