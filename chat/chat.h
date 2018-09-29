#include "tcpsocket.h"
#include "udpsocket.h"

typedef struct Chat{
    tcpServer m_TcpServer;
    tcpClient m_TcpClient;
    udpServer m_UdpServer;
    udpClient m_UdpClient;
    char ipAddress[MINSIZE];
    char connectedIP[MAXSIZE][MINSIZE];
    int connectedFlag;
}chat;

bool InitChat(chat *chat);
bool DestroyChat(chat *chat);
bool ConnectToFriend(chat *chat,char *ipAddress);
void WaitForFriend(chat *chat);
void WaitForChat(chat *chat);
bool WaitForConfirmation(chat *chat,char *ipAddress);
