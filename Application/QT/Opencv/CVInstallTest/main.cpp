#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main()
{
    cout << "sssss" << endl;
    string path = "./a.jpg";
    Mat image = imread(path);

    if(!image.data)
    {
        cout << "图片加载失败" << endl;
        return -1;
    }



    namedWindow("origin");
    imshow("origin", image);

    int clos = (image.cols - 1) * image.channels();
    int rows = image.rows;  //行数， 高度

    cout << image.cols << "=======" << image.channels()  <<  "       rows:" << rows  << endl;

    int offsetx = image.channels();
    Mat dst = Mat::zeros(image.size(), image.type());
    for(int row = 1; row < (rows - 1) ; row++)
    {
        const uchar * current = image.ptr<uchar>(row);
        const uchar * previouts = image.ptr<uchar>(row -1);
        const uchar * next = image.ptr<uchar>(row + 1);
        uchar * output = dst.ptr<uchar>(row);

        //saturate_cast<uchar>(-100),返回 0
        //saturate_cast<uchar>(288),返回255
        //saturate_cast<uchar>(100),返回100
        //这个函数的功能是确保RGB值得范围在0~255
        for(int col = offsetx; col < clos; col++)
        {
            output[col] = saturate_cast<uchar> ( 5 * current[col] - (current[col - offsetx]  + current[col + offsetx] + previouts[col] + next[col]));
        }
    }




    namedWindow("contrast image demo", CV_WINDOW_AUTOSIZE);
    imshow("contrast image demo", dst);




     Mat gray;
     cvtColor(image, gray, COLOR_RGBA2GRAY);
     namedWindow("gray");
     imshow("gray", gray);
     imwrite("./pa.jpeg", gray);


     //1.定义掩膜:Mat kernel = (Mat_<char>(3,3) << 0, -1, 0, -1, 5, -1, 0, -1, 0)
     //2.filter2D(src, dst, src.depth(), kernel); 其中src与dst是mat类型变量， src.depth表示位图深度，有32， 24， 8等
     Mat kernel = (Mat_<char>(3,3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
     Mat dst2 = Mat::zeros(image.size(), image.type());
     filter2D(image, dst2, image.depth(), kernel );
     namedWindow("yan mo cao zuo");
     imshow("yan mo cao zuo", dst2);


     //图像腐蚀
     Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
     Mat dstImage;
     erode(image, dstImage, element);
     imshow("腐蚀效果", dstImage);

     //图像模糊
     Mat dstImage2;
     blur(image, dstImage2, Size(217,217));
     imshow("tu xiang mo hu", dstImage2);

     waitKey(0);

    return 0;
}
