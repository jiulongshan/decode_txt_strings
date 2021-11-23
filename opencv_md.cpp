/*
 * @Author: your name
 * @Date: 2021-10-22 10:51:00
 * @LastEditTime: 2021-10-26 15:28:06
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /opencv_input_file/opencv_md.cpp
 */
#include "opencv2/opencv.hpp"
#include<iostream>
using namespace std;
using namespace cv;

bool MoveDetect(Mat norm, Mat frame, vector<Rect>& boundRect)
{
    // Mat result = frame2.clone();
    Mat gray1, gray2;
    cvtColor(norm, gray1, CV_BGR2GRAY);
    cvtColor(frame, gray2, CV_BGR2GRAY);

    Mat diff;
    absdiff(gray1, gray2, diff);
    // imshow("absdiss", diff);
    threshold(diff, diff, 5, 255, CV_THRESH_BINARY);
    // imshow("threshold", diff);

    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
    Mat element2 = getStructuringElement(MORPH_RECT, Size(9, 9));
    erode(diff, diff, element);
    // imshow("erode", diff);

    dilate(diff, diff, element2);
    // imshow("dilate", diff);

    vector<vector<Point>> contours;
    vector<Vec4i> hierarcy;
    findContours(diff, contours, hierarcy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    cout<<"num="<<contours.size()<<endl;
    for(int i=0; i<contours.size(); i++){
        boundRect.push_back(boundingRect((Mat)contours[i]));
    }

    if(boundRect.size()>0)
        return true;
    return false;
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
        resize(frame, frame, Size(640, 360));
        if(frame.empty())
            break;
        else{
            count++;
            if(count==1)
                background = frame.clone(); //提取第一帧为背景帧
            // imshow("video", frame);
            vector<Rect> boundRect;
            MoveDetect(background, frame, boundRect);
            //其余代码相同
            int x0=0, y0=0, w0=0, h0=0;
            // vector<Rect> boundRect(contours.size()); //定义外接矩形集合
            printf("%d\n", boundRect.size());
            for(int i=0; i<boundRect.size(); i++)
            {
                // boundRect[i] = boundingRect((Mat)contours[i]); //查找每个轮廓的外接矩形
                // boundRect.push_back(boundingRect((Mat)contours[i]));
                x0 = boundRect[i].x;  //获得第i个外接矩形的左上角的x坐标
                y0 = boundRect[i].y; //获得第i个外接矩形的左上角的y坐标
                w0 = boundRect[i].width; //获得第i个外接矩形的宽度
                h0 = boundRect[i].height; //获得第i个外接矩形的高度
                //筛选
                if(w0*h0>3000)
                    rectangle(frame, Point(x0, y0), Point(x0+w0, y0+h0), Scalar(0, 255, 0), 2, 8); //绘制第i个外接矩形
            }
            background = frame.clone(); //update background
            imshow("frame", frame);
            if(waitKey(50)==27)
               break;
        }
    }
    cap.release();
}