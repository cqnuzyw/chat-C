#include <stdio.h>
#include "chat.h"

int main()
{
    printf("正在初始化程序...");
    chat app;
    InitChat(&app);

    fd_set socket;
    struct timeval timer;
    int flag,maxlen;
    while(1){
        FD_ZERO(&socket);
        FD_SET(0, &socket);
        maxlen= 0;
        FD_SET(app.m_UdpServer.m_connect, &socket);
        if(maxlen<app.m_UdpServer.m_connect)
            maxlen=app.m_UdpServer.m_connect;
        timer.tv_sec = 3;
        timer.tv_usec = 0;

        flag = select(maxlen+1, &socket, NULL, NULL, &timer);
        system("clear");
        if(flag == -1){
            printf("Select Error!\n");
            break;
        }else if(flag == 0){
            puts("欢迎进入聊天室！");
            system("ip addr show |grep inet | grep brd | cut -d ' ' -f 6");
            puts("<连接c><退出q>:");
            continue;
        }else{
            if(FD_ISSET(app.m_UdpServer.m_connect,&socket)){
                struct sockaddr_in addr;
                char buffer[MAXSIZE];
                memset(buffer, 0 ,sizeof(buffer));
                socklen_t len=sizeof (addr);
                ssize_t num=recvfrom(app.m_UdpServer.m_connect,buffer,MAXSIZE,0,(struct sockaddr *)&addr,&len);
                char *ipAddress=inet_ntoa(addr.sin_addr);
                if(app.m_TcpServer.m_connect<0){
                    if(!strcmp(buffer,"connect")){
                        while(1){
                            printf("%s 想要连接(y/n):",ipAddress);
                            char ch=getchar();
                            while(getchar()!='\n');
                            if(ch=='y'){
                                SendMessage(&app.m_UdpClient,ipAddress,"true");
                                WaitForFriend(&app);
                                ConnectToFriend(&app,ipAddress);
                                WaitForChat(&app);
                                break;
                            }
                            if(ch=='n'){
                                SendMessage(&app.m_UdpClient,ipAddress,"false");
                                break;
                            }
                        }
                    }
                    if(!strcmp(buffer,"free")){
                        printf("%s 已空闲！\n",ipAddress);
                        sleep(2);
                    }
                }
                else{
                    SendMessage(&app.m_UdpClient,ipAddress,"false");
                }
            }
            if(FD_ISSET(0, &socket)){
                char ipAddress[MINSIZE];
                switch(getchar()){
                case 'c':
                    printf("输入ip地址:");
                    scanf("%s",ipAddress);
                    if(WaitForConfirmation(&app,ipAddress)){
                        ConnectToFriend(&app,ipAddress);
                        WaitForFriend(&app);
                        WaitForChat(&app);
                    }
                    break;
                case 'q':
                    return 0;
                }
                while(getchar()!='\n');
            }
        }
    }
    DestroyChat(&app);
    return 0;
}
