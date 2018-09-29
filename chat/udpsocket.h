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
#define UdpPORT 1111

typedef struct UdpServer{
    struct sockaddr_in m_socket;
    int m_connect;
    int flag;
}udpServer;

typedef struct UdpClient{
    struct sockaddr_in m_socket;
    int m_connect;
}udpClient;

bool InitUdpServer(udpServer *server);
bool DealMessage(udpServer *server);
bool DestroyUdpServer(udpServer *server);


bool InitUdpClient(udpClient *client);
bool SendMessage(udpClient *client,char *ipAddress,char *message);
bool DestroyUdpClient(udpClient *client);
