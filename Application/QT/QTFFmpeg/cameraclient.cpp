#include "cameraclient.h"

cameraclient::cameraclient()
{
   m_videoID = 0;
   m_recvDataToggle = false;
   m_recvCommandToggle = false;

   pthread_mutex_init(&mutex_cRecv, nullptr);
   pthread_mutex_init(&mutex_cSend, nullptr);
   pthread_mutex_init(&mutex_dRecv, nullptr);
   pthread_mutex_init(&mutex_dSend, nullptr);

}


bool cameraclient::startTCPSocketConnection(CC_NetConnectInfo *connectionInfo)
{
    printf("IP %s \n ",connectionInfo->server_ip);
    printf("PORT %d \n", connectionInfo->port);
    printf("USER %s \n", connectionInfo->user_name);
    printf("PASS %s\n", connectionInfo->pass_word);

    memcpy(m_IPAdress, connectionInfo->server_ip, sizeof(m_IPAdress));
    memcpy(m_userName, connectionInfo->user_name, sizeof (m_userName));
    memcpy(m_password, connectionInfo->pass_word, sizeof(m_password));
    m_port = connectionInfo->port;

    int ret = initTCPSocketConnection(connectionInfo);

    if(ret == 0)
    {

    }
}

bool cameraclient::stopTCPSocketClient()
{

}

int cameraclient::initTCPSocketConnection(CC_NetConnectInfo *connecttionInfo)
{
    printf("\n\n 数据传输socket:初始化数据传输Socket....");
    memset(&m_dSockaddr_in, 0, sizeof(struct sockaddr));
    m_cSockfd = socket(AF_INET, SOCK_STREAM, 0);

    printf("数据传输Socket socket ID %d \n", m_cSockfd);

    if(m_dSockfd < 0)
    {
        printf("socket error \n");
        return  -1;
    }

    m_cSockaddr_in.sin_family=AF_INET;
    m_cSockaddr_in.sin_port=htons(connecttionInfo->port);
    m_cSockaddr_in.sin_addr.s_addr=htons(INADDR_ANY);

    if(inet_pton(AF_INET,m_IPAdress,&m_cSockaddr_in.sin_addr.s_addr)<=0)
    {
        printf("inet_pton error!!!\n");
        return -1;
    }

    //非阻塞的connect模式
    int flags = fcntl(m_cSockfd, F_GETFL, 0);
    fcntl(m_cSockfd, F_SETFD, flags|O_NONBLOCK);
    int connectRet = connect(m_cSockfd, (struct sockaddr *)&m_cSockaddr_in, sizeof (struct sockaddr));
    printf("socket connection result %d \n", connectRet);

    //设置回阻塞模式

}

void cameraclient::setUpdateGUICallback(UpdateVideo2GUI_Callback callback, unsigned long userData)
{
    m_updateVideoCallback = callback;
    u_userData = userData;
}
