#include "opencv2/opencv.hpp"
#include<iostream>
#include <unistd.h>
#include <stdlib.h>
using namespace std;
using namespace cv;

//int to string helper function
string intToString(int number)
{
    stringstream ss;
    ss << number;
    return ss.str();
}

Mat MoveDetect(Mat img)
{
    Mat result = img.clone();
    Mat gray ;//= Mat::zeros(dHeight,dWidth,CV_32FC3);
    Mat avg = img;//cv::Mat::zeros(img.size(), CV_32FC3); ;
    std::cout<<"img depth "<<img.depth()<<endl;
    std::cout<<"gray depth "<<gray.depth()<<endl;

    cvtColor(img, gray, CV_BGR2GRAY);
    imshow("gray", gray);
    sleep(3);
    return img;
    std::cout<<"gray depth "<<gray.depth()<<endl;

    GaussianBlur(gray, gray, cv::Size(21, 21), 0);
    std::cout<<"gray depth "<<gray.depth()<<endl;

    Mat diff;
    // accumulateWeighted(gray, avg, 0.5);
    convertScaleAbs(avg, avg);
    absdiff(gray, avg, diff);
    threshold(diff, diff, 5, 255, CV_THRESH_BINARY);
    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
    dilate(diff, diff, element);
    imshow("threshold", diff);
    // blur(diff, diff, Size(10, 10));//均值滤波
    imshow("blur", diff);

    vector<vector<Point>> contours;  
    vector<Vec4i> hierarcy;
    findContours(diff, contours, hierarcy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE); //查找轮廓
    vector<Rect> boundRect(contours.size()); //定义外接矩形集合
    //drawContours(img2, contours, -1, Scalar(0, 0, 255), 1, 8);  //绘制轮廓
    //其余代码相同
    int x0=0, y0=0, w0=0, h0=0;
    for(int i=0; i<contours.size(); i++)
    {
        if(contourArea(contours[i]) < 5000){
            continue;
        }
        boundRect[i] = boundingRect((Mat)contours[i]); //查找每个轮廓的外接矩形
        
        x0 = boundRect[i].x;  //获得第i个外接矩形的左上角的x坐标
        y0 = boundRect[i].y; //获得第i个外接矩形的左上角的y坐标
        w0 = boundRect[i].width; //获得第i个外接矩形的宽度
        h0 = boundRect[i].height; //获得第i个外接矩形的高度
        //筛选
        // if(w0>30 && h0>30)
        rectangle(result, Point(x0, y0), Point(x0+w0, y0+h0), Scalar(0, 255, 0), 2, 8); //绘制第i个外接矩形
    }    
    return result;
}

int main(int argc, char* argv[])
{     
    if(argc != 2){
        printf("usage: ./otest dir\n");
        return -1;
    }
    
    std::string input_dir = std::string(argv[1]);    
    printf("argc = %d, input_dir = %s\n", argc, input_dir.c_str());

    VideoCapture cap(input_dir.c_str());
    if(!cap.isOpened()) //检查打开是否成功
         return -1;
    Mat frame;
    Mat result;
    Mat background;
    int count=0;
    while(1)
    {
        cap>>frame;
        if(frame.empty())
            break;
        else{            
            count++;
            if(count==1)
                 background = frame.clone(); //提取第一帧为背景帧
            imshow("video", frame);
            result = MoveDetect(frame);
            imshow("result", result);
            if(waitKey(50)==27)
               break;
        }
    }
    cap.release();  
}