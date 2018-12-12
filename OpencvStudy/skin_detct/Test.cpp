////
////  Test.cpp
////  OpencvStudy
////
////  Created by wangzy on 2018/11/13.
////  Copyright © 2018 wangzy. All rights reserved.
////
//
//#include <stdio.h>
//#include "headers.hpp"
//#include "Histogram1D.hpp"
//#include "Tool.hpp"
//
//void on_mouse(int EVENT, int x, int y, int flags, void* userdata);
//
//
//int main(){
//    
//    Mat mat=imread("/Users/wangzy/Pictures/test2_s.png");
//
//
//    cout<<"wh:"<<mat.cols<<" "<<mat.rows<<endl;
//    cout<<"chanel:"<<mat.channels()<<endl;
//    cout<<"dim:"<<mat.dims<<endl;
//
//    Mat_<Vec3b>::iterator it = mat.begin<Vec3b>();
//    Mat_<Vec3b>::iterator itend = mat.end<Vec3b>();
//    int total=0;
//    while (it != itend)
//    {
//        total++;
////        cout<<"-->:"<<(*it)<<endl;
//        //(*it)[0]=0;//b
//        //(*it)[1]=0;//g
//        //(*it)[2]=0;//r
//        //do something with (*it)[0] (*it)[1] (*it)[2]
////        cout<<"b:"<<(int)(*it)[0]<<" g:"<<(int)(*it)[1]<<" r:"<<(int)(*it)[2]<<endl;
//        it++;
//    }
////
////    cout<<"total:"<<total<<endl;
////
//    
//    
////    cv::Mat image = cv::imread("puppy.jpg");
//    string windowName="img";
//    imshow(windowName, mat);
//    setMouseCallback(windowName, on_mouse,&mat);
//    
//    Histogram1D ch;
//    cv::Mat histo = ch.getHistogram(mat);
//    for (int i = 0; i < 256; i++)
//    {
//        std::cout << "Value " << i << " = " << histo.at<float>(i) << std::endl;
//    }
//
//    waitKey();
//    return 0;
//}
//
//
//
////获取鼠标点击的颜色像素
//void on_mouse(int EVENT, int x, int y, int flags, void* userdata)
//{
//    Mat hh;
//    hh = *(Mat*)userdata;
//    Point p(x, y);
//    switch (EVENT)
//    {
//        case EVENT_LBUTTONDOWN:
//        {
//            
//            printf("b=%d\t", hh.at<Vec3b>(p)[0]);
//            printf("g=%d\t", hh.at<Vec3b>(p)[1]);
//            printf("r=%d\n", hh.at<Vec3b>(p)[2]);
////            circle(hh, p, 2, Scalar(255),3);
//            long count= Tool::findBGRCount((int)hh.at<Vec3b>(p)[0], (int)hh.at<Vec3b>(p)[1], (int)hh.at<Vec3b>(p)[2], hh,5);
//            cout<<"same is:"<<count<<endl;
//        }
//            break;
//            
//    }
//    
//}
