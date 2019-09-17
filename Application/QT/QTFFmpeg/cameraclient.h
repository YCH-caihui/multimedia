#ifndef CAMERACLIENT_H
#define CAMERACLIENT_H
#include "cctcpdatadefine.h"
#include "h264decoder.h"
#include <QDebug>
typedef void (*UpdateVideo2GUI_Callback)(RGBData_Define * rgbData, unsigned long userData);
class cameraclient
{
public:
    cameraclient();
    ~cameraclient();

    bool startTCPSocketConnection(CC_NetConnectInfo * connectionInfo);
    bool stopTCPSocketClient();
    void setUpdateGUICallback(UpdateVideo2GUI_Callback callback, unsigned long userData);
    int detachThreadCreate(pthread_t * thread, void * start_routine, void * arg);

    void doLoginAuthentication();
    bool sendUserLoginRequest();

    bool sendCSocketData(char * pBuf, int aLength);
    bool recvCSocketData(char * pBuf, int aLength);

    int recvUserLoginReply();

private:  char                        m_IPAdress[64];                 //camIP
    int                         m_port;                       //端口
    char                        m_userName[13];                  //用户名
    char                        m_password[13];                  //密码

    int                         m_cSockfd;                      //命令套接字.
    int                         m_dSockfd;                      //数据通道套接字.

    int                         m_videoID;      //视频ID

    struct   sockaddr_in        m_cSockaddr_in;
    struct   sockaddr_in        m_dSockaddr_in;

    bool                        m_recvDataToggle;
    bool                        m_recvCommandToggle;

    pthread_mutex_t             mutex_cRecv;
    pthread_mutex_t             mutex_cSend;
    pthread_mutex_t             mutex_dRecv;
    pthread_mutex_t             mutex_dSend;

    unsigned long u_userData;
    UpdateVideo2GUI_Callback m_updateVideoCallback;

    int initTCPSocketConnection(CC_NetConnectInfo * connecttionInfo);

    static void* doLoginAuthenticationThread(void* arg);
};

#endif // CAMERACLIENT_H
