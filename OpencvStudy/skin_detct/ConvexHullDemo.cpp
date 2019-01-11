//
//
//#include <stdio.h>
//#include "headers.hpp"
//#include "Histogram1D.hpp"
//#include "Tool.hpp"
//
//using namespace std;
//
//int main(){
//
// RNG rng(12345);
//
//VideoCapture cam(0);
//if(!cam.isOpened())
//    return 0;
//
//
//     Mat sourceImg;
//    while(true){
//
//        cam>>sourceImg;
//        flip(sourceImg, sourceImg, 1);//画图像左右纠正
//
//        Mat skinMat=Mat(sourceImg.size(), sourceImg.type());
//
//        Tool::getSkinOutput(sourceImg, skinMat);//h获取皮肤部分
//
//        //
////        Mat threshold_output=Mat(skinMat.size(),CV_8UC1);
//        vector<vector<Point>>contours;
//        vector<vector<Point>>contoursTarget;
//        vector<Vec4i>hierarchy;
//
//        findContours(skinMat, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));//寻找轮廓
//
//        //寻找目标轮廓
//        vector<vector<Point>> hulls;
//
//        for (int i = 0; i < contours.size(); i++)
//        {
//
//            Rect r=boundingRect(contours[i]);
//            if(r.width>40 && r.height>40 && (r.height>r.width)){
//
//                Tool::drawRectangle(skinMat, r);
//                contoursTarget.push_back(contours[i]);
//
//                vector<Point> hull;
//
//                convexHull(Mat(contours[i]),hull, false);
//
//                hulls.push_back(hull);
//            }
//        }
//
//
//
//
//
//        Mat drawing = Mat::zeros(skinMat.size(), CV_8UC3);
//        for (int i = 0; i < contoursTarget.size(); i++)
//        {
//            Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
//            drawContours(drawing, contoursTarget, i, color, 1, 8, vector<Vec4i>(), 0, Point());
//            drawContours(drawing, hulls, i, color, 1, 8, vector<Vec4i>(), 0, Point());
//        }
//
//
//        imshow("src", sourceImg);
//        imshow("skin", skinMat);
//        imshow("result", drawing);
//
//
//                if(27 == waitKey(10)){
//                    return 0;
//                }
//    }
//
//return 0;
//
//}
