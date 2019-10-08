#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <stdio.h> //文件输入输出
#include <stdlib.h>//最常用的系统函数包含了C语言标准库函数的定义
#include <sys/stat.h> //Unix/Lunus系统定义文件状态所在的伪标准头文件
#include <string.h> //字符串
#include <stdbool.h> //bool
#include <signal.h> //信号
#include <sys/types.h> //基本系统数据类型
#include <netinet/in.h> //sockeaddr_in
#include <sys/socket.h> //socket 函数结构
#include <arpa/inet.h> //IP地址转换
#include <netdb.h>  //提供设置 获取域名
#include <sys/ioctl.h>  //对IO控制的函数
#include <sys/poll.h> //socket 等待测试机制函数
#include <sys/wait.h> //等待
#include <unistd.h>  //提供通用文件，目录，程序以及进程的函数
#include <errno.h>  //错误号定义
#include <fcntl.h> //对文件控制
#include <err.h>  //获取导致函数调用失败的原因
#include <pthread.h> //线程操作
#include <time.h> //时间函数
#include <net/if.h> //配置ip地址，激活接口，配置MTU等
#include <ifaddrs.h> //获取本地网络接口信息

#define LISTEN_PORT 30000 //端口号

void start_tcp_server();

#endif // SOCKETSERVER_H
