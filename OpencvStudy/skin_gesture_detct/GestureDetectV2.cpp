////手势识别
////#include "headers.hpp"
//#include "../utils/headers.hpp"
//#include "Tool.hpp"
//
//Mat input_image;
//Mat output_mask;
//Mat output_image;
//Mat mask;
//
//int resizeRect=240;//图像大小（采集图像，处理特征的时候归一化大小）
//int sacnWidnowSize=resizeRect;
//vector<vector<Point>> hulls;
//vector<string> labelVectorTotal;
//map<long,string> labelMap;
//map<string, vector<float>> getMLdata();
//string getTargetLabel(string srourceFilePath);
//
////======================================================================================================================================================
//int main()
//{
//
//
//
//    //加载机器学习数据
//    Ptr<KNearest> knn= KNearest::create();
//    knn->setDefaultK(3);
//    knn->setIsClassifier(true);//设置该算法用来分类而不是线性回归
//
//    map<string,vector<float>> dataMap =getMLdata();
//    map<string,vector<float>>::iterator it;
//
//
//    it = dataMap.begin();
//    long dataMatCol=it->second.size();//生成学习数据列
//    size_t dataMatRow=dataMap.size();
//
//    vector<vector<float>> dataMatVectros;
//
//    vector<string> labelMatVector;
//    //    vector<string> labelMatStringVector;
//
//    Mat dataMat;
//
//
//    while(it != dataMap.end())
//    {
//        cout<<it->first<<" "<<it->second.size()<<" features "<<endl;
//
//        vector<float> dataItem=it->second;
//
//        Mat dataItemMat(1,(int)dataItem.size(),CV_32F);
//
//        for(int j=0,jsize=(int)dataItem.size();j<jsize;j++){//特征单个vector转换成 1行多列的Mat
//            dataItemMat.at<float>(0, j)=dataItem.at(j);
//        }
//
//        dataMat.push_back(dataItemMat);//把所有的单个Mat 组装成个一个大的训练集Mat
//
//        labelMatVector.push_back(it->first);//所有的数据label
//
//        it ++;
//    }
//
//
//    //第二种labelMat
//    Mat labelMatV2;
//
//    for(int j=0,jsize=(int)labelMatVector.size();j<jsize;j++){//label的vector转Mat
//
//        string label=labelMatVector.at(j);
//        size_t pos=label.find_last_of("/");
//        string name=label.substr(pos+1,8);
//        int labelInt=(int)Tool::string2Int(name);
//        labelMatV2.push_back(labelInt);
//        labelMap[labelInt]=label;
//
//    }
//
//
//    //第二种方式labelMat未文件名的整数
//    Ptr<TrainData> tData= TrainData::create(dataMat, ROW_SAMPLE, labelMatV2);
//
//    knn->train(tData);
//
//    cout<<"总的机器学习数据行:"<<dataMatRow<<" 列："<<dataMatCol<<endl;
//
//
//    //一切准备工作完成，打开摄像头开始识别图片
//
//    VideoCapture cam(0);
//    if(!cam.isOpened())
//        return 0;
//
//
//    double fps;
//    char fpst[32];  // 用于存放帧率的字符串
//    double t = 0;
//
//    RNG rng(12345);
//
//    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//
//    //Hog特征采集器，用来采集摄像头图片区域的特征，这里的参数必须和CraeteMLData中采集数据时一样，不然数据特征纬度对不上，无法识别
//    HOGDescriptor *hog=new HOGDescriptor(Size(sacnWidnowSize,sacnWidnowSize),
//                                         Size(sacnWidnowSize/2,sacnWidnowSize/2),
//                                         Size(sacnWidnowSize/4,sacnWidnowSize/4),
//                                         Size(sacnWidnowSize/4,sacnWidnowSize/4),9);
//
//
//
//    while(true)
//    {
//
//        hulls.clear();
//
//        t = (double)cv::getTickCount();//帧率开始
//        cam >> input_image;
//
//        flip(input_image, input_image, 1);//图片反转，MAC OS上opecv采集的图片是f左右相反的，这里特殊处理一下
//
//        if(input_image.empty())
//            return 0;
//
//
//        //新建一个空Mat 存放皮肤区域数据
//        output_mask = Mat::zeros(input_image.size(), CV_8UC1);
//        //采集皮肤区域
//        Tool::getSkinOutput(input_image, output_mask);
//
//
//        vector< vector<Point> > contours;   // 轮廓
//        vector< vector<Point> > filterContours; // 筛选后的轮廓
//        vector< Vec4i > hierarchy;    // 轮廓的结构信息
//        contours.clear();
//        hierarchy.clear();
//        filterContours.clear();
//
//        //开始寻找轮廓
//        findContours(output_mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
//
//        vector<vector<Point>> hull(contours.size());
//        // 去除一些没有用的区域（比如太小，或者比例和手脸偏差太大的区域，这些区域大概率不是人体面部区域）
//        for (size_t i = 0; i < contours.size(); i++)
//        {
//
//            //找到这些区域后，用矩形框标记出来
//            Rect r=boundingRect(contours[i]);
//
////            float whs=r.width*1.0f/r.height;//宽高比
//            float hws=r.height*1.0/r.width;//高宽比，其实用一个比例就可以了
//
//            //计算向量面积，也是一种排出法，排出更多的非肤色区域
//            long abs=fabs(contourArea(Mat(contours[i]))) ;
//
////            int ow=output_mask.cols;
////            int oh=output_mask.rows;
//
//
//            if((r.width>100 && r.height>100)/*&&(whs>=0.6f && whs<=1.6f)*/&&(abs> 1000) && hws>1.0f ){
//
//                //把r符合面部区域特征的区域保存下来
//
//                //新建一个resizeRect大小的区域，用来保存符合肤色区域数据，并且要把这块区域全部resize 采样时候的大小
//                //就是resizeRect的大小，方便取得一样维度的特征，而且并不会影响识别结果（为什么不会，这里自己要想清楚）
//                Mat srcMat=output_mask(r);
//                Mat newMat(resizeRect,resizeRect,srcMat.type());
//
//                resize(srcMat, newMat, Size(resizeRect,resizeRect));//归一化，都变成240X240大小
//
//                vector<float> tempdescripters;//采集的特征放到了这里
//                hog->compute(newMat, tempdescripters);//采集这块图片的特征
//                Mat inputHogMat(1,(int)dataMatCol,CV_32F);
//                //用这个for循环把特征vector变成Mat
//                for(int j=0,jsize=(int)tempdescripters.size();j<jsize;j++){
//                    inputHogMat.at<float>(0, j)=tempdescripters.at(j);
//                }
//
//                //识别结果，因为是KNN所有无论如何总会得到一个结果，这个结果必然是LabelMat中的一个
//
//                Mat results;
//                float response=knn->predict(inputHogMat,results);//得到的结果必然是LablMat中的一个
//
//                string srourceFilePath=labelMap[(long)response];//匹配到的数据样本文件
//                string target=getTargetLabel(srourceFilePath);//数据样本文件夹
//
//                string result=target;//对应成文字
//
//
//                if(/*result.compare("face")!=0 &&*/ result.compare("cabinet")!=0){
//
//                    //并且把这些区域标记出来
//                    Tool::drawRectangle(output_image, r);
//                    Tool::drawRectangle(output_mask, r);
//                    Tool::drawRectangle(input_image,r);
//
//
//                    filterContours.push_back(contours[i]);
//
//
//                    //在多种窗口中绘制出来
//                    Tool::drawText(r.tl(), result, output_mask);
//                    Tool::drawText(r.tl(), result, input_image);
//                    Tool::drawText(r.tl(), result, output_image);
//                    cout<<"resultmat:"<<results<<endl;
//
//                    cout<<"结果result:"<<" response: "<<result<<" path:"<<srourceFilePath<<endl;
//
//                    //只会知右手胜利手势的凸包
//                    if(result.compare("right_hand_v")==0){
//
//                         vector<Point> hull;
//                         convexHull(Mat(contours[i]),hull, false);
//                         hulls.push_back(hull);
//
//                         for(size_t p=0,psize=hull.size();p<psize;p++){
//                            Tool::drawCircle(input_image, hull[p], 10);
//                         }
//                    }
//
//                }
//
//            }
//
//        }
//
//        output_mask.setTo(0);//使用前重置一下
//        //绘制轮廓
//        drawContours(output_mask, filterContours, -1, Scalar(255,0,0), CV_FILLED); //8, hierarchy);//把肤色区域绘制进去
//
//        //绘制目标区域轮廓，凸包
//        for(size_t x=0,xsize=hulls.size();x<xsize;x++){
//            drawContours(output_image, hulls, x, color, 1, 8, vector<Vec4i>(), 0, Point());
//            drawContours(input_image, hulls, x, color, 1, 8, vector<Vec4i>(), 0, Point());
//        }
//
//        input_image.copyTo(output_image, output_mask);//成功把肤色区域拷贝到output_image
//
//        //====================
//        t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
//        fps = 1.0 / t;
//        sprintf(fpst, "fps:%.2f", fps);      // 帧率保留两位小数
////        cout<<"fps:"<<fps<<endl;
//        //帧率结束
//        //以下是绘制帧率，目前帧率比较少，这个demo自然还有很多优化空间
//        imshow("output_mask", output_mask);//Mask窗口
//        Tool::drawText(Point(100,100), string(fpst), output_mask);
//
//        imshow("input_image", input_image);//原始图
//        Tool::drawText(Point(100,100), string(fpst), input_image);
//
//        imshow("output_image", output_image);//输出图
//        Tool::drawText(Point(100,100), string(fpst), output_image);
//        //窗口太多会y影响帧率
//
//        output_image.setTo(0);
//
//        if(27 == waitKey(10)){
//            delete hog;
//            return 0;
//        }
//
//    }
//    return 0;
//}
//
//
//
//
//
//
////从数据文件夹中生成训练数据,这里的数据可以通过CreateMLdata.cpp采集后，人工区别后放到ml_data下对应的文件夹中
//map<string, vector<float>> getMLdata(){
//
//
////这是存放采样数据的地方，这里不方便使用使用执行文件的相对路径，所有换了文件夹，要修改project 下ml_data的路径
//string rootdir="/Users/wangzy/study/opencv/OpencvStudy/OpencvStudy/ml_data";
//
//vector<string> files=Tool::listFiles(rootdir);
//map<string, vector<float>> dataMap;
//
////注意这里的HOG特征参数，扫描窗口越小，越准确，但是也越耗时
//HOGDescriptor *hog=new HOGDescriptor(Size(sacnWidnowSize,sacnWidnowSize),
//                                     Size(sacnWidnowSize/2,sacnWidnowSize/2),
//                                     Size(sacnWidnowSize/4,sacnWidnowSize/4),
//                                     Size(sacnWidnowSize/4,sacnWidnowSize/4),9);
//
//for(size_t i=0,isize=files.size();i<isize;i++){
//    string file=files.at(i);
//    string absPath=string(rootdir+"/"+file);
//
//    cout<<absPath<<endl;
//
//    vector<string> subFiles=Tool::listFiles(absPath);
//    if(!subFiles.empty()){
//        cout<<"subfile count:"<<subFiles.size()<<endl;
//
//        for(size_t j=0,jsize=subFiles.size();j<jsize;j++){
//            string imgAbsPath=absPath+"/"+subFiles.at(j);
//
//            labelVectorTotal.push_back(imgAbsPath);
//
//            Mat mat=imread(imgAbsPath);
//
//            Mat newMat(resizeRect,resizeRect,CV_32F);
//
//            resize(mat, newMat, Size(resizeRect,resizeRect));//归一化，都变成240X240大小后再采集特征
//
//            vector<float> descripters;
//            hog->compute(newMat, descripters);
//            dataMap[imgAbsPath] = descripters;//存放到Map 中去
//        }
//
//    }
//}
//
//delete hog;
//return dataMap;
//}
//
//
////获取匹配到的文件
//string getTargetLabel(string srourceFilePath){
//
//    size_t pos=srourceFilePath.find_last_of("/");
//    size_t pos2=srourceFilePath.substr(0,pos).find_last_of("/");
//    string target=srourceFilePath.substr(0,pos).substr(pos2+1);
//    return target;
//}
//
