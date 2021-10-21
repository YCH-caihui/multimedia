#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
int main()
{
    /*
    //1.读入视频
    VideoCapture capture;
    capture.open("./ap01.flv");
    //2.循环显示每一帧
    while(1)
    {
         Mat frame; //定义一个map变量，用于存储每一帧图像
         capture >> frame; //读取当前桢
         //诺视频播放完成，退出循环
         if(frame.empty())
         {
             break;
         }
         imshow("read video", frame);
         waitKey(30);
    }
    */

    //1.从摄像头读入视频
    VideoCapture capture(0);

    Mat edges;
    //2循环显示每一帧
    while(1)
    {
        Mat frame; //定义一个Mat变量，用于存储每一帧的图像
        capture >> frame;


        //将原图像转换成灰度图像
        cvtColor(frame, edges, COLOR_BGR2GRAY);

        //使用3x3内核来降噪
        blur(edges, edges, Size(7, 7));//进行模糊

        //进行Canny边缘检测并显示
        Canny(edges, edges, 0, 30, 3);


        imshow("读取视频", edges);//显示当前帧
        waitKey(30);//延时30ms
    }
    return 0;
}
