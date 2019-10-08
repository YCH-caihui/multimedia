#include "socketserver.h"

static int m_ListenSocket = -1;
static int m_BindSocket = -1;



void start_tcp_server()
{
    m_ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(m_ListenSocket < 0) //返回值小于0 则失败
    {
        close(m_ListenSocket);
        m_ListenSocket = -1;
    }

    struct sockaddr_in  server_addr;
    bzero(&server_addr, sizeof(struct sockaddr_in)); //初始化sockaddr_in
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(LISTEN_PORT); //主机字节序转换为网络字节序
    server_addr.sin_addr.s_addr = INADDR_ANY;

    //绑定
    int nRet = bind(m_ListenSocket, reinterpret_cast<const struct sockaddr *>(&server_addr), sizeof (struct sockaddr_in));
    if(nRet < 0)
    {
        close(m_ListenSocket);
        m_ListenSocket = -1;
    }

    //监听
    nRet = listen(m_ListenSocket, 10);
    if(nRet < 0)
    {
        close(m_ListenSocket);
        m_ListenSocket = -1;
    }



}
