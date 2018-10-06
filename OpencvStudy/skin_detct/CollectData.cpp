////用来采集，面部，左右手，手势
//#include "headers.hpp"
//#include "Tool.hpp"
//
//Mat input_image;
//Mat output_mask;
//Mat output_image;
//Mat mask;
//
//
//string output_mask_window ="output_mask_window";
//string output_image_window ="output_image_window";
//string input_image_window="input_image_window";
//

//int main()
//{
//VideoCapture cam(0);
//
//
//if(!cam.isOpened())
//    return 0;
//
//double fps;
//char fpst[32];  // 用于存放帧率的字符串
//double t = 0;
//
//RNG rng(12345);
//
////    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//
//while(true)
//{
//
//    t = (double)cv::getTickCount();//帧率开始
//
//    cam >> input_image;
//
//    flip(input_image, input_image, 1);
//
//    if(input_image.empty())
//        return 0;
//
//    output_mask = Mat::zeros(input_image.size(), CV_8UC1);
//
//    Tool::getSkinOutput(input_image, output_mask);
//
//
//    vector< vector<Point> > contours;   // 轮廓
//    vector< vector<Point> > filterContours; // 筛选后的轮廓
//    vector< Vec4i > hierarchy;    // 轮廓的结构信息
//    contours.clear();
//    hierarchy.clear();
//    filterContours.clear();
//
//    findContours(output_mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);//寻找轮廓，联通区域
//
//    vector<vector<Point>> hull(contours.size());
//    // 去除伪轮廓
//    for (size_t i = 0; i < contours.size(); i++)
//    {
//
//        Rect r=boundingRect(contours[i]);
//
//        float whs=r.width*1.0f/r.height;//宽度比高度
//        float hws=r.height*1.0/r.width;
//
//        long abs=fabs(contourArea(Mat(contours[i]))) ;//计算面积绝对值
//
//        if((r.width>100 && r.height>100)/*&&(whs>=0.6f && whs<=1.6f)*/&&(abs> 1000) && hws>0.9f ){
//            filterContours.push_back(contours[i]);
//            Tool::drawRectangle(output_image, r);
//        }
//    }
//    
//    output_mask.setTo(0);
//    
//    drawContours(output_mask, filterContours, -1, Scalar(255,0,0), CV_FILLED); //8, hierarchy);
//    
//    input_image.copyTo(output_image, output_mask);
//    
//    
//    cout<<"最终轮廓："<<filterContours.size()<<endl;
//    for (size_t i = 0; i < filterContours.size(); i++)
//    {
//        Rect r=boundingRect(filterContours[i]);
////        Tool::drawRectangle(output_mask, r);
//
//        char sumtxt[32];
//    
//        sprintf(sumtxt, "/Users/wangzy/temp/%ld.png", Tool::getTimeClock());
//        
//        imwrite(string(sumtxt), output_mask(r));
//    
//
//        Tool::drawRectangle(input_image, r);
//        
//    }
//    
//    
//    
//
////        drawContours(input_image, filterContours, -1, Scalar(255,0,0), CV_FILLED); //8, hierarchy);
//    
//    
//    //====================
//    t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
//    fps = 1.0 / t;
//    sprintf(fpst, "fps:%.2f", fps);      // 帧率保留两位小数
//           //帧率结束
//
//    Tool::drawText(Point(100,100), string(fpst), output_mask);
//
//
////    imshow(output_mask_window, output_mask);
////        imshow(input_image_window, input_image);
//    imshow(output_image_window, output_image);
//
//    output_image.setTo(0);
//    if(27 == waitKey(30))
//        return 0;
//}
//return 0;
//}
