#include "chat.h"

bool InitChat(chat *chat)
{
    chat->m_TcpClient.m_connect=-1;
    chat->m_TcpServer.m_connect=-1;
    chat->m_UdpServer.m_connect=-1;
    chat->m_UdpClient.m_connect=-1;
    chat->m_TcpServer.m_listen=-1;
    chat->connectedFlag=0;
    if(!InitTcpServer(&chat->m_TcpServer)){
        return false;
    }
    if(!InitTcpClient(&chat->m_TcpClient)){
        return false;
    }
    if(!InitUdpServer(&chat->m_UdpServer)){
        return false;
    }
    if(!InitUdpClient(&chat->m_UdpClient)){
        return false;
    }
    return true;
}

bool DestroyChat(chat *chat)
{
    if(!DestroyTcpServer(&chat->m_TcpServer)){
        return false;
    }
    if(!DestroyTcpClient(&chat->m_TcpClient)){
        return false;
    }
    if(!DestroyUdpServer(&chat->m_UdpServer)){
        return false;
    }
    if(!DestroyUdpClient(&chat->m_UdpClient)){
        return false;
    }
    return true;
}

void WaitForFriend(chat *chat)
{
    while(1){
        chat->m_TcpServer.m_connect=AcceptClient(&chat->m_TcpServer);
        if(chat->m_TcpServer.m_connect)break;
    };
}

bool ConnectToFriend(chat *chat, char *ipAddress)
{
    return ConnectToServer(&chat->m_TcpClient,ipAddress);
}

void WaitForChat(chat *chat)
{
    memcpy(chat->ipAddress,GetSockAddress(chat->m_TcpClient.m_connect),sizeof(chat->ipAddress));
    fd_set rfds;
    struct timeval tv;
    int retval, maxfd;
    while(1){
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        maxfd = 0;
        FD_SET(chat->m_TcpServer.m_connect, &rfds);
        if(maxfd<chat->m_TcpServer.m_connect)
            maxfd=chat->m_TcpServer.m_connect;
        FD_SET(chat->m_UdpServer.m_connect, &rfds);
        if(maxfd<chat->m_UdpServer.m_connect)
            maxfd=chat->m_UdpServer.m_connect;
        tv.tv_sec = 1;
        tv.tv_usec = 0;

        retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
        if(retval == -1){
            printf("select出错，客户端程序退出\n");
            break;
        }else if(retval == 0){
            //printf("服务端没有任何输入信息，并且客户端也没有信息到来，waiting...\n");
            continue;
        }else{
            if(FD_ISSET(chat->m_UdpServer.m_connect,&rfds)){
                struct sockaddr_in addr;
                char buffer[MAXSIZE];
                memset(buffer, 0 ,sizeof(buffer));
                socklen_t len=sizeof (addr);
                ssize_t num=recvfrom(chat->m_UdpServer.m_connect,buffer,MAXSIZE,0,(struct sockaddr *)&addr,&len);
                char *ipAddress=inet_ntoa(addr.sin_addr);
                if(!strcmp(buffer,"free")){
                    printf("%s 已空闲！\n",ipAddress);
                }
                if(!strcmp(buffer,"connect")){
                    SendMessage(&chat->m_UdpClient,ipAddress,"false");
                    memcpy(chat->connectedIP[chat->connectedFlag],ipAddress,MINSIZE);
                    chat->connectedFlag++;
                }
            }
            /*客户端发来了消息*/
            if(FD_ISSET(chat->m_TcpServer.m_connect,&rfds)){
                char buffer[1024];
                memset(buffer, 0 ,sizeof(buffer));
                ssize_t len = recv(chat->m_TcpServer.m_connect, buffer, sizeof(buffer), 0);
                if(!strcmp(buffer, "exit\n")) break;
                if(strcmp(buffer, "\n"))printf("%s:\n\t%s",GetPeerAddress(chat->m_TcpClient.m_connect), buffer);
            }
            /*用户输入信息了,开始处理信息并发送*/
            if(FD_ISSET(0, &rfds)){
                //printf("%s:\n\t",GetSockAddress(chat->m_TcpClient.m_connect));
                char buf[1024];
                fgets(buf, sizeof(buf), stdin);
                send(chat->m_TcpClient.m_connect, buf, sizeof(buf), 0);
                if(strcmp(buf, "exit\n") == 0) break;
            }
        }
    }
    DestroyTcpClient(&chat->m_TcpClient);
    close(chat->m_TcpServer.m_connect);
    chat->m_TcpServer.m_connect=-1;
    InitTcpClient(&chat->m_TcpClient);
    for(int i=0;i<chat->connectedFlag;i++){
        SendMessage(&chat->m_UdpClient,chat->connectedIP[i],"free");
        memset(chat->connectedIP[i],0,MINSIZE);
    }
    chat->connectedFlag=0;
}

bool WaitForConfirmation(chat *chat,char *ipAddress)
{
    if(!SendMessage(&chat->m_UdpClient,ipAddress,"connect")){
        return false;
    }
    if(!DealMessage(&chat->m_UdpServer)){
        printf("%s 正忙...\n<返回b>:",ipAddress);
        while(getchar()!='b');
        return false;
    }
    return true;
}
