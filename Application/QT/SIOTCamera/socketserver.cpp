#include "socketserver.h"

static int m_ListenSocket = -1;
static int m_BindSocket = -1;



int detach_thread_create(pthread_t *thread, void * start_routine, void *arg)
{
    pthread_attr_t attr;
    pthread_t thread_t;
    int ret = 0;

    if(thread==NULL){
        thread = &thread_t;
    }
    //初始化线程的属性
    if(pthread_attr_init(&attr))
    {
        printf("pthread_attr_init fail!\n");
        return -1;
    }

    //设置线程detachstate属性。该表示新线程是否与进程中其他线程脱离同步
    if(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED))
    {//新线程不能用pthread_join()来同步，且在退出时自行释放所占用的资源。
        printf("pthread_attr_setdetachstate fail!\n");
        goto error;
    }

    ret = pthread_create(thread, &attr, (void *(*)(void *))start_routine, arg);
    if(ret < 0){
        printf("pthread_create fail!\n");
        goto error;
    }

    //将状态改为unjoinable状态，确保资源的释放。
   ret =  pthread_detach(thread_t);

error:
    pthread_attr_destroy(&attr);

    return ret;
}

//非阻塞套接字发送
int tcp_send(int socketFd, char * sendBuffer, unsigned int sendLen)
{
    if(socketFd < 0)
    {
        return -1;
    }

    if(sendBuffer == NULL || sendLen <= 0)
    {
        return -1;
    }

    fd_set write_set;
    struct timeval tmval;
    memset(&write_set, 0, sizeof(tmval));

    int nRet = 0;
    int sendSIze = 0;

    while(sendSIze < sendLen)
    {
        FD_ZERO(&write_set);
        FD_SET(socketFd, &write_set);

        tmval.tv_sec = 3;
        tmval.tv_usec = 0;
        nRet = select(socketFd + 1, &write_set, NULL, NULL, &tmval);
        if(nRet == 0)
        {
            usleep((10 * 1000));
            continue;
        }

        if(nRet < 0 && nRet == EINTR)
        {
            usleep(10 * 1000);
            continue;
        }

        if(nRet < 0)
        {
            return -1;
        }

        if(FD_ISSET(socketFd, &write_set))
        {

      RESENDBUFFER:
            nRet = send(socketFd, sendBuffer + sendSIze, sendLen - sendSIze, 0);

            if(nRet > 0)
            {
                sendLen -= nRet;
                sendSIze += nRet;
            }

            if(nRet < 0 && errno == EINTR)
            {
                goto RESENDBUFFER;

            }

        }
    }
}

//非阻塞套接字收
int tcp_receive(int socketFd, char * pBuffer, unsigned int nSize)
{
    if(pBuffer == NULL || (nSize < 0))
    {
        return -1;
    }

    if(socketFd < 0)
    {
        return -1;
    }


     int ret = 0;
     fd_set read_set;
     struct timeval tmval;
     memset(&tmval, 0 , sizeof(tmval));

     int recvedSize = 0;


     while(recvedSize < nSize)
     {
         FD_ZERO(&read_set);
         FD_SET(socketFd, &read_set);

         tmval.tv_sec = 10;
         tmval.tv_usec = 0;

         ret = select(socketFd + 1, &read_set, NULL, NULL, &tmval);
         if(ret == 0) //超时
         {
             usleep(10 * 1000);
             continue;
         }

         if(ret == -1)
         {
             return -1;
         }

         if(FD_ISSET(socketFd, &read_set))
         {
             ret = recv(socketFd, pBuffer + recvedSize, nSize - recvedSize, 0);
             if(ret < 0  && (errno == EAGAIN || errno == EINTR))
             {
                 continue;
             }

             if(ret <= 0)
             {
                 return -3;
             }

             recvedSize += ret;
         }
     }
}

int set_socket_params(int sockfd)
{
    fcntl(sockfd, F_SETFL, 0, O_NONBLOCK);
//    struct timeval tmval;
//    tmval.tv_sec = 5;
//    tmval.tv_usec = 0;
//    int size = 5;

//    setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF,  &size, sizeof (size));

    return 0;
}


int process_client_command()
{

}

int client_socketProcess(struct sockaddr_in * ipaddr)
{
    fd_set read_set;
    int wait_count = 0;

    set_socket_params(m_BindSocket);

    struct timeval tmval;
    tmval.tv_sec = 20;
    tmval.tv_usec = 0;

    FD_ZERO(&read_set);
    FD_SET(m_BindSocket, &read_set);

    while (wait_count < 10)
    {
        int ret = select(m_BindSocket + 1, &read_set, nullptr, nullptr, &tmval);
        switch (ret)
        {
           case -1:
            break;
           case 0:
            wait_count++;
            break;
        default:
            if(FD_ISSET(m_BindSocket, &read_set))
            {
                //recv.....
            }
        }
    }

}

void accept_process()
{
    struct sockaddr_in cline_socket;
    socklen_t size;
    while(1)
    {
        size = sizeof(struct sockaddr_in);
        m_BindSocket = accept(m_ListenSocket, reinterpret_cast<struct sockaddr *>(&cline_socket), &size);
        if(m_BindSocket > 0)
        {
            printf("连接成功%d \n", m_BindSocket);
            //继续处理....数据收发.... recv  send ......

        }

        close(m_BindSocket); //父进程关闭

        usleep(2000 * 1000);
    }
}

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

    accept_process();


}
