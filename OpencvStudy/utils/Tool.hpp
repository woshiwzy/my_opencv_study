
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

using namespace  std;
using namespace cv;
using namespace ml;

namespace Tool {


long getTimeLabel();
void drawRectangle(Mat& img,Rect box);
void drawText(Point point,string label,Mat& mat);
vector<string> listFiles(string dir);    
int sumWhiteColorCount(Mat &src);
float sumWhiteColorCountPercent(Mat &src);

void drawCircle(Mat &img, Point center, int radius);

void drawText(Point point,string label,Mat& resultMat){


    putText(resultMat, label, point, CV_FONT_HERSHEY_PLAIN, 2.0f, Scalar(255,100,255));

}


void printTime(){
    time_t tt = time(NULL);//这句返回的只是一个时间cuo
    tm* t= localtime(&tt);
    printf("%d-%02d-%02d %02d:%02d:%02d\n",
           t->tm_year + 1900,
           t->tm_mon + 1,
           t->tm_mday,
           t->tm_hour,
           t->tm_min,
           t->tm_sec);
    
    printf("now:%li",getTimeLabel());
}

long getTimeLabel(){
    time_t now_time;
    now_time = time(NULL);
    
    return now_time;
}


long getTimeClock(){

 long x=(long)   clock();

    return x;
    
}

void drawRectangle(Mat& img,Rect box){
    
    rectangle(img, box.tl(), box.br(), Scalar(200,100,88));
    
}

void drawRectangle(Mat& img,Rect box,Scalar scalar){
    
    rectangle(img, box.tl(), box.br(), scalar);
    
}

void drawRectangle(Mat& img,Rect box,Scalar scalar,int thickless){
    
    rectangle(img, box.tl(), box.br(), scalar,thickless);
    
}


vector<string> listFiles(string dirRoot){
    
    vector<string> subfiles;
    DIR    *dir;
    struct  dirent    *ptr;
    dir = opendir(dirRoot.c_str()); ///open the dir
    while((ptr = readdir(dir)) != NULL) ///read the list of this dir
    {

        
        string fname=string(ptr->d_name);
        if("."!=fname && ".."!=fname && ".DS_Store"!=fname){
             subfiles.push_back(ptr->d_name);
        }

    }
    closedir(dir);
    return subfiles;
}


void drawCircle(Mat &img, Point center, int radius)
{
    circle(img, center, radius,Scalar(200,100,88));
    
}

void drawCircle(Mat &img, Point center, int radius,Scalar scalar)
{
    circle(img, center, radius,scalar);
    
}
void drawCircle(Mat &img, Point center, int radius,Scalar scalar,int thickless)
{
    circle(img, center, radius,scalar,thickless);
    
}


//检测皮肤并且输出
void getSkinOutput(Mat &input_image,Mat& output_mask){
    
    if(input_image.size().width!=output_mask.size().width ||
       input_image.size().height!=output_mask.size().height){
        
        cout<<"error:output_mask = Mat::zeros(input_image.size(), CV_8UC1);"<<endl;
        
        return;
        
    }
    
    Mat skinCrCbHist = Mat::zeros(Size(256, 256), CV_8UC1);
    ellipse(skinCrCbHist, Point(113, 155.6), Size(23.4, 15.2), 43.0, 0.0, 360.0, Scalar(255, 255, 255), -1);
    Mat element = getStructuringElement(MORPH_RECT, Size(4, 4), Point(-1, -1) );
    
    Mat ycrcb_image;
    
    output_mask = Mat::zeros(input_image.size(), CV_8UC1);
    
    cvtColor(input_image, ycrcb_image, CV_BGR2YCrCb); //首先转换成到YCrCb空间
    
    for(int i = 0; i < input_image.rows; i++) //利用椭圆皮肤模型进行皮肤检测
    {
        uchar* p = (uchar*)output_mask.ptr<uchar>(i);
        Vec3b* ycrcb = (Vec3b*)ycrcb_image.ptr<Vec3b>(i);
        for(int j = 0; j < input_image.cols; j++)
        {
            if(skinCrCbHist.at<uchar>(ycrcb[j][1], ycrcb[j][2]) > 0){
                p[j] = 255;
            }
            
        }
    }
    
    morphologyEx(output_mask,output_mask,MORPH_CLOSE,element);
    
}


void getRgbPercent(Mat &image,Scalar &rgb){
    
    for(int i=0;i<image.rows;i++){
        for(int j=0;j<image.cols;j++)
        {
            
            Vec3b b3vec =image.at<Vec3b>(i,j);
            
            int b=image.at<Vec3b>(i,j)[0];
            int g=image.at<Vec3b>(i,j)[1];
            int r=image.at<Vec3b>(i,j)[2];
            
            
        }
    }
}
    
    
    
    /*
     统计二值化后白像素个数
     */
    int sumWhiteColorCount(Mat &src)
    {
        
        int counter = 0;
        //迭代器访问像素点
        Mat_<uchar>::iterator it = src.begin<uchar>();
        Mat_<uchar>::iterator itend = src.end<uchar>();
        for (; it!=itend; ++it)
        {
            if((*it)>0) counter+=1;//二值化后，像素点是0或者255
        }
        return counter;
    }
    
    /*
     统计二值化后白像素个数
     */
    float sumWhiteColorCountPercent(Mat &src)
    {
        
        int counter = 0;
        //迭代器访问像素点
        Mat_<uchar>::iterator it = src.begin<uchar>();
        Mat_<uchar>::iterator itend = src.end<uchar>();
        for (; it!=itend; ++it)
        {
            if((*it)>0) counter+=1;//二值化后，像素点是0或者255
        }
        
        int total=src.cols*src.rows;
        return counter*1.0/total;
    }

}

