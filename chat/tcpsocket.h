#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>

#define MAXSIZE 1024
#define MINSIZE 16
#define TcpPORT 2222

typedef struct TcpServer{
    struct sockaddr_in m_socket;
    int m_connect;
    int m_listen;
}tcpServer;

typedef struct TcpClient{
    struct sockaddr_in m_socket;
    int m_connect;
}tcpClient;

bool InitTcpServer(tcpServer *server);
int AcceptClient(tcpServer *server);
bool DestroyTcpServer(tcpServer *server);


bool InitTcpClient(tcpClient *client);
bool ConnectToServer(tcpClient *client,char *ipAddress);
bool DestroyTcpClient(tcpClient *client);

char *GetPeerAddress(int connect);
char *GetSockAddress(int connect);
