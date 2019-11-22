#ifndef MSGDEFINE_H
#define MSGDEFINE_H

#define MJCAM_DEVICE_MAX_NUM  9

#define    CONTROLLCODE_LOGINREQUEST                0    //登陆请求  (操作命令)
#define    CONTROLLCODE_LOGINREPLY                  1    //登陆回应  (操作命令)
#define    CONTROLLCODE_VERIFIYREQUEST              2    //验证请求  (操作命令)
#define    CONTROLLCODE_VERIFIYREPLY                3    //验证回应  (操作命令)
#define    CONTROLLCODE_VIDEOTRANSLATION_REQUEST    4    //视频请求  (操作命令)
#define    CONTROLLCODE_VIDEOTRANSLATION_REPLY      5    //视频回应  (操作命令)
#define    CONTROLLCODE_VIDEOTRANSLATION_STOP       6    //视频停止  (操作命令)

#define    CONTROLLCODE_LISTENSTART_COMMAND         8    //开始音频  (操作命令)
#define    CONTROLLCODE_LISTENSTART_REPLY           9    //音频回应  (操作命令)
#define    CONTROLLCODE_LISTENSTOP_COMMAND          10   //停止音频  (操作命令)



#define    CONTROLLCODE_SEARCH_BROADCAST_REQUEST    0   //广播请求
#define    CONTROLLCODE_SEARCH_BROADCAST_REPLY      1   //广播回应

#define    CONTROLLCODE_DECODER_CONTROLL            14   //解码器      (操作命令)
#define    CONTROLLCODE_REPLY_ALARM                 25   //报警        (操作命令)
#define    CONTROLLCODE_MULTIVIDEOINFOREPLY         28   //多路信息操   (操作命令)

#define    CONTROLLCODE_VIDEOTRANSPORTCOMMD         0   //视频传输请   (传输命令)
#define    CONTROLLCODE_VIDEOTRANSPORTREPLY         1   //视频传输回应   (传输命令)
#define    CONTROLLCODE_LISTENTRANSPORTREPLY        2   //监听回应操作码      (传输命令)
#define    CONTROLLCODE_TALKTRANSPORTREPLY          3   //对讲回应操作码      (传输命令)


#define    CONTROLLCODE_USERDATA_REQ               32
#define    CONTROLLCODE_USERDATA_REPLY             33

#define    CONTROLLCODE_LASER_CTRL_REQ             34
#define    CONTROLLCODE_LASER_CTRL_REPLY           35
#define    CONTROLLCODE_AIR_QUALITY_REQ            36
#define    CONTROLLCODE_AIR_QUALITY_REPLY          37
#define    CONTROLLCODE_WRITE_UID_REQ              38
#define    CONTROLLCODE_WRITE_UID_REPLY            39
#define    CONTROLLCODE_SET_WIFI_REQ               40
#define    CONTROLLCODE_SET_WIFI_REPLY             41
#define    CONTROLLCODE_GET_SD_REQ                 42
#define    CONTROLLCODE_GET_SD_REPLY               43

#define    CONTROLLCODE_KEEPALIVECOMMAND            255

#define    LOGINREQUESTREPLYVALUE_OK                0
#define    LOGINREQUESTREPLYVALUE_BADACCESS         1

#define     VERIFYREPLYRETURNVALUE_OK               0   //较验正确
#define     VERIFYREPLYRETURNVALUE_USER_ERROR       1   //用户名出错
#define     VERIFYREPLYRETURNVALUE_PASS_ERROR       5   //密码出错

#define     VIDEOIREQUESTREPLY_OK                   0   //同意连接
#define     VIDEOIREQUESTREPLY_USERFULL             2   //用户已经满
#define     VIDEOIREQUESTREPLY_FORBIDEN             8   //禁止连接


typedef struct CC_MessageHeader
{
    unsigned char messageHeader[4]; //x协议头 摄像头操作协议： "CCTC"摄像头传输协议 "CCTD"
    short controlMask; // 操作码，区分同一协议中的不同命令
    unsigned char reseved0;  // 保留 默认= 0；
    unsigned char reserved1[8]; //保留
    int commandLength;  //命令中正文的长度
    int rewserved2; //保留
} CC_MsgHeader;

//登陆响应结构体
typedef struct LoginRequestReply
{
    short           result;   //返回 0 OK,2 已经达到最大连接许可，连接将断开.
    unsigned char   devID[13];  //返回设备id.
    unsigned char   reserved0[4];  //保留
    unsigned char   reserved1[4];  //保留
    unsigned char   devVersion[4];  //摄像头系统固件版本.

}CC_LoginRequestReply;



#endif // MSGDEFINE_H
