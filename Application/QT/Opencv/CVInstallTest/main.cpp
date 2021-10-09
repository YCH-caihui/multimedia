#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main()
{
    cout << "sssss" << endl;
    string path = "./a.jpg";
    Mat image = imread(path);
    namedWindow("origin");
    imshow("origin", image);

     Mat gray;
     cvtColor(image, gray, COLOR_RGBA2GRAY);
     namedWindow("gray");
     imshow("gray", gray);
     waitKey(0);

    return 0;
}
