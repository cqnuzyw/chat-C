#include "tcpsocket.h"

bool InitTcpServer(tcpServer *server){
    if((server->m_listen=socket(AF_INET, SOCK_STREAM,0))==-1){
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        return false;
    }
    memset(&server->m_socket,0,sizeof(server->m_socket));
    server->m_socket.sin_family = AF_INET;
    server->m_socket.sin_addr.s_addr = htonl(INADDR_ANY);
    server->m_socket.sin_port = htons(TcpPORT);
    if(bind(server->m_listen,(struct sockaddr*)&server->m_socket,sizeof(server->m_socket))==-1){
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        return false;
    }
    if(listen(server->m_listen, 10) == -1){
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        return false;
    }
    printf("OpenServer!\n");
    return true;
}

bool DestroyTcpServer(tcpServer *server)
{
    close(server->m_listen);
    close(server->m_connect);
    server->m_listen=-1;
    server->m_connect=-1;
    printf("CloseServer!\n");
    return true;
}

bool InitTcpClient(tcpClient *client)
{
    if((client->m_connect=socket(AF_INET, SOCK_STREAM, 0))<0){
       printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
       return false;
    }
    memset(&client->m_socket, 0, sizeof(client->m_socket));
    client->m_socket.sin_family = AF_INET;
    client->m_socket.sin_port = htons(TcpPORT);
    printf("OpenClient!\n");
    return true;
}

bool ConnectToServer(tcpClient *client,char *ipAddress)
{
    client->m_socket.sin_addr.s_addr=inet_addr(ipAddress);
    if(connect(client->m_connect,(struct sockaddr*)&client->m_socket,sizeof(client->m_socket))<0){
        printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
        return false;
    }
    printf("Connect to Server!\n");
    return true;
}

bool DestroyTcpClient(tcpClient *client)
{
    close(client->m_connect);
    client->m_connect=-1;
    printf("CloseClient!\n");
    return true;
}

int AcceptClient(tcpServer *server)
{
    socklen_t len=sizeof(server->m_socket);

    int fd=accept(server->m_listen,(struct sockaddr*)&server->m_socket,&len);
    if(fd<0){
        printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
    }
    printf("AcceptClient!\n");
    return fd;
}

char *GetPeerAddress(int connect)
{
    struct sockaddr_in socket;
    socklen_t len=sizeof (socket);
    getpeername(connect,(struct sockaddr*)&socket,&len);
    return inet_ntoa(socket.sin_addr);
}

char *GetSockAddress(int connect)
{
    struct sockaddr_in socket;
    socklen_t len=sizeof (socket);
    getsockname(connect,(struct sockaddr*)&socket,&len);
    return inet_ntoa(socket.sin_addr);
}
