#include "udpsocket.h"

bool InitUdpServer(udpServer *server)
{
    if((server->m_connect=socket(AF_INET,SOCK_DGRAM,0))==-1){
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        return false;
    }
    memset(&server->m_socket,0,sizeof(server->m_socket));
    server->m_socket.sin_family = AF_INET;
    server->m_socket.sin_port = htons(UdpPORT);
    server->m_socket.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(server->m_connect,(struct sockaddr*)&server->m_socket,sizeof(server->m_socket))==-1){
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        return false;
    }
    printf("OpenServer!\n");
    return true;
}

bool DestroyUdpServer(udpServer *server)
{
    close(server->m_connect);
    server->m_connect=-1;
    printf("CloseServer!\n");
    return true;
}

bool InitUdpClient(udpClient *client)
{
    if((client->m_connect=socket(AF_INET,SOCK_DGRAM, 0))<0){
       printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
       return false;
    }
    memset(&client->m_socket, 0, sizeof(client->m_socket));
    client->m_socket.sin_family = AF_INET;
    client->m_socket.sin_port = htons(UdpPORT);
    printf("OpenClient!\n");
    return true;
}

bool SendMessage(udpClient *client, char *ipAddress,char *message)
{
    client->m_socket.sin_addr.s_addr=inet_addr(ipAddress);

    ssize_t len=sendto(client->m_connect,message,10,0,(struct sockaddr *)&client->m_socket,sizeof (client->m_socket));

    return true;
}

bool DestroyUdpClient(udpClient *client)
{
    close(client->m_connect);
    client->m_connect=-1;
    printf("CloseClient!\n");
    return true;
}

bool DealMessage(udpServer *server)
{
    struct sockaddr_in addr;
    char buffer[MAXSIZE];
    memset(buffer, 0 ,sizeof(buffer));
    socklen_t len=sizeof (addr);
    ssize_t num=recvfrom(server->m_connect,buffer,MAXSIZE,0,(struct sockaddr *)&addr,&len);
    if(strcmp(buffer,"true")){
        return false;
    }
    return true;
}
