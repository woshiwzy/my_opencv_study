//#include <iostream>
//#include <opencv2/opencv.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/core/core.hpp>
//#include <stdio.h>
//#include <cstdlib>
//
//using namespace std;
//using namespace cv;
//
//void my_mouse_callback(int event,int x, int y, int flags, void *param);
//
//CvPoint st_point;
//int blue  = 0;
//int green = 0;
//int red   = 0;
//CvFont font;
//
//int main(int argc, const char * argv[]) {
//    const char filename[] = "/Users/linwang/Desktop/Lena.png";
//    
//    IplImage * image = cvLoadImage(filename);
//    
//    cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 1, 1, 0, 2, 8);
//    
//    cvNamedWindow("Lena");
//    
//    cvSetMouseCallback("Lena",  my_mouse_callback, (void *)image);
//    
//    cvShowImage("Lena", image);
//    
//    cvWaitKey(0);
//    
//    cvReleaseImage(&image);
//    
//    cvDestroyWindow("Lena");
//}
//
//void my_mouse_callback(int event,int x,int y,int flags,void *param)
//{
//    IplImage * image = (IplImage *)param;
//    
//    switch (event) {
//            /*鼠标离开*/
//        case CV_EVENT_LBUTTONDOWN:
//        {
//            /*鼠标左键点击事件，记录点击的（x，y）*/
//            st_point.x = x;
//            st_point.y = y;
//            cout<<"按下左键 ： "<<" x = "<<x <<" y "<<y<<endl;
//            break;
//        }
//        case CV_EVENT_LBUTTONUP:
//        {
//            /*鼠标左键放开事件，读取当前点的像素值BGR*/
//            unsigned char * ptr = (uchar *)(image->imageData + st_point.x * image->widthStep);
//            blue  = (int)(*(ptr + st_point.y));
//            green = (int)(*(ptr + st_point.y + 1));
//            red   = (int)(*(ptr + st_point.y + 2));
//            cout<<"抬起左键 ： "<<" blue= "<<blue<<" green = "<<green<<" red =  "<<red<<endl;
//            char text[20] = "0";
//            sprintf(text,"(%d,%d,%d)",blue,green,red);
//            cout<<"Text = "<<text<<endl;
//            cvPutText(image, text, st_point, &font, CV_RGB(0,0,0));
//            cvShowImage("Lena", image);
//            break;
//        }
//        default:
//            break;
//    }
//}
//
