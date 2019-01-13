//
//
//
///**
// 这个类用来生成样本数据
// **/
//
//#include "NumberRec.hpp"
//#define window_name "window"
//#define window_name_dst "window_dst"
//#define window_name_th "window_th"
//#define location_save "/Users/wangzy/study/opencv_300/my_data/%ld.png"//保存样本到指定的位置
//
//
//int pen_width =10;
//int line_type =3;
//
//int window_width=128;
//int window_height=128;
//
//
//void on_MouseHandle(int event,int x,int y,int flags,void* param);
//
//
//bool g_bDrawingBox=false;
//RNG g_rng(12345);
//
//
//
//int main(int argc, const char * argv[]) {
//
//
//    
//
//
//    Mat srcImage(window_width,window_height,CV_8UC1),tempImage;
//
//    srcImage.copyTo(tempImage);
//
//    namedWindow(window_name);
//
//    setMouseCallback(window_name, on_MouseHandle,(void*)&srcImage);
//
//
//    cout<<"要运行效果的话，请注视掉这个类文件的Main方法，然后打开Demo.cpp的注视"<<endl;
//    cout<<"按C清除画板:"<<endl;
//    cout<<"按S键保存并清除手写结果:"<<endl;
//
//
//    while(1){
//
//
//
//
//        srcImage.copyTo(tempImage);
//
//        imshow(window_name, tempImage);
//
//        char key= waitKey();
//        switch (key) {
//
//            case 'c':
//                srcImage=Scalar::all(0);
//                break;
////            case '+':
////                pen_width+=5;
////                break;
////
////            case '-':
////                pen_width-=5;
////                if(pen_width<5){
////                    pen_width=5;
////                }
////                break;
//            case 's':
//
//                char saved_name[100];
//
//
//                sprintf(saved_name, location_save, Tool::getTimeLabel());
//
//                vector<vector<Point>> contours;
//
//                srcImage.copyTo(tempImage);
//
//
//                // find
//                findContours(tempImage,contours,CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE);
//
//                Rect rect;
//
//
//                for(size_t i=0;i!=contours.size();++i){
//
//                    Rect  tempRect=boundingRect(contours[i]);
//                    //find max rect
//                    if(tempRect.width>rect.width && tempRect.height>rect.height){
//                        rect=tempRect;
//                    }
//
//                }
//
//                if(rect.width==0){
//
//                    cout<<"请先写数字再按‘S’按键报错"<<endl;
//                    continue;
//                }
//
//
//                Mat resultCenter=  Mat(window_width,window_height,CV_8UC1);
//                resultCenter=Scalar::all(0);
//
//                Mat tempArea;
//
//                srcImage(rect).copyTo(tempArea);
//
//
//                Rect resultRIORect;
//
//                int nlt=window_width/2-rect.width/2;
//                int nll=window_height/2-rect.height/2;
//
//
//                //把内容区域拷贝到新图像的中间
//
//                Rect nrect;
//
//                nrect.x=nlt;
//                nrect.y=nll;
//
//                nrect.width=rect.width;
//                nrect.height=rect.height;
//
//
//
//                tempArea.copyTo(resultCenter(nrect));
//                Tool::drawRectangle(srcImage,rect);
//                imshow(window_name, srcImage);
//
//                imwrite(saved_name, resultCenter);
//
//                cout<<"保存到:"<<saved_name<<endl;
//
//                srcImage=Scalar::all(0);
//
//                break;
//
//        }
//
//
//        if(waitKey(10)==27)
//            break;
//    }
//
//    return 0;
//}
//
//
//
//
//
//
//Point lastPoint(-1,-1);
//
//bool isPress=false;
//
//void on_MouseHandle(int event,int x,int y,int flags,void* param){
//
//
//    Mat& image=*(Mat*)param;
//
//    switch (event) {
//
//        case EVENT_MOUSEMOVE:
//
//            if(lastPoint.x==-1){
//                lastPoint.x=x;
//                lastPoint.y=y;
//            }else{
//                if(isPress){
//                    Point nowPoint(x,y);
//                    line(image, lastPoint, nowPoint, Scalar(255,255,255),pen_width,line_type);
//                    imshow(window_name, image);
//                    lastPoint=nowPoint;
//                }
//
//            }
//
//            break;
//
//        case EVENT_LBUTTONDOWN:
//            isPress=true;
//            lastPoint.x=x;
//            lastPoint.y=y;
//            break;
//
//        case EVENT_LBUTTONUP:
//            isPress=false;
//            break;
//    }
//
//}
