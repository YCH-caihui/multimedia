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
    qDebug("IP %s \n ",connectionInfo->server_ip);
    qDebug("PORT %d \n", connectionInfo->port);
    qDebug("USER %s \n", connectionInfo->user_name);
    qDebug("PASS %s\n", connectionInfo->pass_word);

    memcpy(m_IPAdress, connectionInfo->server_ip, sizeof(m_IPAdress));
    memcpy(m_userName, connectionInfo->user_name, sizeof (m_userName));
    memcpy(m_password, connectionInfo->pass_word, sizeof(m_password));
    m_port = connectionInfo->port;

    int ret = initTCPSocketConnection(connectionInfo);

    if(ret == 0)
    {
        detachThreadCreate(nullptr, (void *)doLoginAuthenticationThread, (void *)this);
    }

    return true;
}

void * cameraclient::doLoginAuthenticationThread(void *arg) {
    cameraclient * camClient = (cameraclient *)arg;
    if(camClient != nullptr) {
        camClient->doLoginAuthentication();
    }

    return nullptr;
}

void cameraclient::doLoginAuthentication(){
    qDebug("登入认证中。。。。。\n");

    int error = 0;
    fd_set r_set, w_set;
    FD_ZERO(&r_set);
    FD_ZERO(&w_set);

    FD_SET(m_cSockfd, &r_set);
    FD_SET(m_cSockfd, &w_set);

    struct timeval timout = {10, 0};
    int retValue = select(m_cSockfd+1, &r_set, &w_set, nullptr, &timout);

    switch (retValue) {
      case -1:
        qDebug("select 系统条用出错\n");
       break;
      case 0:
        qDebug("slect 超时...\n");
        break;
    default:
        //套接字即可读又可写，需要进一步判断
        if(FD_ISSET(m_cSockfd, &r_set) && FD_ISSET(m_cSockfd, &w_set))
        {
            //如果套接口以及可写也可读，需要进一步判断
            socklen_t len = sizeof(error);
            if(getsockopt(m_cSockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
            {
                return ; //获取SO_ERROR属性选项，当然getsockopt
            }
            qDebug("error = %d\n", error);
            if(error != 0)
            {
                //如果error不为0，则表示链接到此没有建立完成
                qDebug("connect failed \n");
                return;
            }
            //如果error为0，则说明链接到此没有建立完成
        }

        //如果套接字可写但是不可读，说明链接完成
        if(!FD_ISSET(m_cSockfd, &r_set) && FD_ISSET(m_cSockfd, &w_set))
        {
            //如果套接口可读可写，则链接完成
            qDebug("connect success\n");

            //登录，认证...
            qDebug("登录，认证.....!\n");

            if(sendUserLoginRequest())
            {
                int ret =recvUserLoginReply();

                //登录成功
                if(ret == 0)
                {
                    m_recvDataToggle = true;
                    m_recvCommandToggle = true;
                    qDebug("开始接受音视频 命令.......\n");

                    //
                }
            }

        }
    }
}

bool cameraclient::sendUserLoginRequest()
{
    CC_MsgHeader loginMsgHeader;
    memset(&loginMsgHeader, 0, sizeof(loginMsgHeader));

    loginMsgHeader.messageHeader[0] = 'C';
    loginMsgHeader.messageHeader[1] = 'C';
    loginMsgHeader.messageHeader[2] = 'T';
    loginMsgHeader.messageHeader[3] = 'C';

    loginMsgHeader.controlMask = CONTROLCODE_LOGIN_REQUEST;

    int loginLength = sizeof (loginMsgHeader);

    if(sendCSocketData((char *)&loginMsgHeader, loginLength))
    {
        return true;
    }

    return false;

}

bool cameraclient::sendCSocketData(char *pBuf, int aLength)
{
    signal(SIGPIPE, SIG_IGN);
    pthread_mutex_lock(&mutex_cSend);

    int sendLen = 0;
    int nRet = 0;

    while(sendLen < aLength)
    {
        nRet = send(m_cSockfd, pBuf, aLength - sendLen, 0);

        if(nRet < 0)
        {
            if(errno == EAGAIN || errno == EINTR || errno == EWOULDBLOCK)
            {
                usleep(10 * 1000);
                continue;
            }
        }

        if(-1 == nRet || 0 == nRet)
        {
            pthread_mutex_unlock(&mutex_cSend);
            qDebug("cSocket send ok %d  %d\n",m_cSockfd, nRet);
            return false;
        }

        sendLen += nRet;
        pBuf += nRet;
        qDebug("cSocket send of  %d  %d \n", m_cSockfd, nRet);
    }

    pthread_mutex_unlock(&mutex_cRecv);
    return true;
}

bool cameraclient::recvCSocketData(char *pBuf, int aLength)
{
    signal(SIGPIPE, SIG_IGN);
    pthread_mutex_lock(&mutex_cRecv);

    int recvLen = 0;
    int nRet = 0;

    while(recvLen < aLength)
    {
        nRet = recv(m_cSockfd, pBuf, aLength - recvLen, 0);

        if(nRet < 0)
        {
            if(errno == EAGAIN || errno == EINTR || errno == EWOULDBLOCK)
            {
                usleep(10 * 1000);
                continue;
            }
        }

        if(-1 == nRet || 0 == nRet)
        {
            pthread_mutex_unlock(&mutex_cRecv);
            qDebug("sScoket recv error");
            return false;
        }

        recvLen += nRet;
        pBuf += nRet;

    }

    pthread_mutex_lock(&mutex_cRecv);
    return true;
}


int cameraclient::recvUserLoginReply()
{
    CC_LoginRequestReply loginReply;
    memset(&loginReply, 0, sizeof(loginReply));
    if(recvCSocketData((char *)&loginReply, sizeof(loginReply)) == false){
        return -1;
    }

    if(strncmp((char *)loginReply.msgHeader.messageHeader, "CCTC",4) == 0)
    {
        if((loginReply.msgHeader.controlMask == CONTROLCODE_LOGIN_REPLY) && (loginReply.result == LOGIN_REQUEST_REPLY_VALUE_OK))
        {
           qDebug("LoginReply INFO Result: %d",loginReply.result); //登录响应结果
           qDebug("LoginReply INFO DEvID: %s \n", loginReply.devID); //设置ID
           qDebug("LoginReply INFO DEvHDVer: %s \n", loginReply.devVersion);  //固件版本
        }
    }

    return 1;
}


bool cameraclient::stopTCPSocketClient()
{

}

int cameraclient::initTCPSocketConnection(CC_NetConnectInfo *connecttionInfo)
{
    qDebug("\n\n 数据传输socket:初始化数据传输Socket....");
    memset(&m_dSockaddr_in, 0, sizeof(struct sockaddr_in));
    m_cSockfd = socket(AF_INET, SOCK_STREAM, 0);

    qDebug("数据传输Socket socket ID %d \n", m_cSockfd);

    if(m_dSockfd < 0)
    {
        qDebug("socket error \n");
        return  -1;
    }

    m_cSockaddr_in.sin_family=AF_INET;  //地址族
    m_cSockaddr_in.sin_port=htons(connecttionInfo->port); // 16位TCP/UDP 端口号
    m_cSockaddr_in.sin_addr.s_addr= htons(INADDR_ANY); //32位IP地址

    if(inet_pton(AF_INET,m_IPAdress,&m_cSockaddr_in.sin_addr.s_addr)<=0)
    {
        qDebug("inet_pton error!!!\n");
        return -1;
    }

//    //非阻塞的connect模式
//    int flags = fcntl(m_cSockfd, F_GETFL, 0);
//    fcntl(m_cSockfd, F_SETFD, flags|O_NONBLOCK);
    int connectRet = connect(m_cSockfd, (struct sockaddr *)&m_cSockaddr_in, sizeof (struct sockaddr));
    qDebug("socket connection result %d \n", connectRet);

//    //设置回阻塞模式
//    int flags1= fcntl(m_cSockfd, F_GETFD, 0);
//    fcntl(m_cSockfd, F_SETFL, flags1&(~O_NONBLOCK));

    struct timeval timeout = {2, 0};

    int nRet1=setsockopt(m_cSockfd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(struct timeval));

        if(nRet1<0){
            qDebug("setsockopt1 error!!!\n");
            return -1;
        }

        int nRet2=setsockopt(m_cSockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(struct timeval));

        if(nRet2<0){
            qDebug("setsockopt2 error!!!\n");
            return -1;
        }

        return 0;

}

int cameraclient::detachThreadCreate(pthread_t * thread, void * start_routine, void * arg)
{
    pthread_attr_t attr;
    pthread_t thread_t;
    int ret = 0;

    if(!thread)
    {
        thread = &thread_t;
    }

    //初始化线程的属性
    if(pthread_attr_init(&attr))
    {
        qDebug("pthread_attr_init fail!\n");
        goto error;
    }

    //设置线程detachState属性，该表示线程是否与进程中其它线程脱离同步
    if(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED))
    {
        //新线程不能使用pthread_joint来同步，且在退出时自行释放占用的资源
        qDebug("pthread_attr_setdetachstate  fail \n");
        goto error;
    }

    ret = pthread_create(thread, &attr, (void *(*)(void *))start_routine, arg);
    if(ret < 0)
    {
        qDebug("pthread_create fail \n");
        goto error;
    }

    //将状态改为joinable状态，确保资源的释放
    ret = pthread_detach(thread_t);
   error:
    pthread_attr_destroy(&attr);
}

void cameraclient::setUpdateGUICallback(UpdateVideo2GUI_Callback callback, unsigned long userData)
{
    m_updateVideoCallback = callback;
    u_userData = userData;
}
