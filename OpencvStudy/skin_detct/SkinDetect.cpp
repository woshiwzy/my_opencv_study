#include "headers.hpp"
#include "Tool.hpp"

Mat input_image;
Mat output_mask;
Mat output_image;
Mat mask;

int resizeRect=240;//图像大小（采集图像，处理特征的时候归一化大小）
int sacnWidnowSize=resizeRect;

//从数据文件夹中生成训练数据
map<string, vector<vector<float>>> getMLdata(){
    
    //    int MAX_PATH=120;
    //    char   buffer[MAX_PATH];
    //    getcwd(buffer, MAX_PATH);
    //    cout<<string(buffer)<<endl;
    //
   
    string rootdir="/Users/wangzy/workspace/opencv_work/OpencvStudy/OpencvStudy/ml_data";
    
    vector<string> files=Tool::listFiles(rootdir);
    map<string, vector<vector<float>>> dataMap;
    
    
    HOGDescriptor *hog=new HOGDescriptor(Size(sacnWidnowSize,sacnWidnowSize),
                                         Size(sacnWidnowSize/2,sacnWidnowSize/2),
                                         Size(sacnWidnowSize/4,sacnWidnowSize/4),
                                         Size(sacnWidnowSize/4,sacnWidnowSize/4),9);
    
    for(size_t i=0,isize=files.size();i<isize;i++){
        string file=files.at(i);
        string absPath=string(rootdir+"/"+file);
        
        cout<<absPath<<endl;
        
        vector<vector<float>> descriptersVector;
        
        vector<string> subFiles=Tool::listFiles(absPath);
        if(!subFiles.empty()){
            cout<<"subfile count:"<<subFiles.size()<<endl;
            
            for(size_t j=0,jsize=subFiles.size();j<jsize;j++){
                string imgAbsPath=absPath+"/"+subFiles.at(j);
                Mat mat=imread(imgAbsPath);
                //             cout<<imgAbsPath<<" "<<mat.rows<<","<<mat.cols;
                Mat newMat(resizeRect,resizeRect,CV_32F);
                
                resize(mat, newMat, Size(resizeRect,resizeRect));//归一化，都变成240X240大小
                
                vector<float> descripters;
                hog->compute(newMat, descripters);
                
                descriptersVector.push_back(descripters);
                
            }
            
            dataMap[file]=descriptersVector;//存放到Map 中去
            
            
        }
    }
    
    delete hog;
    return dataMap;
}

//把不同文件夹所代表的图片名转化为数字，放入label的时候需要
int getLabelMatNumber(string label){
    
//    if(label){
    
        if(label.compare("left_hand")==0){
            return 1;
        }else if(label.compare("left_hand_ok")==0){
            return 2;
        }else if(label.compare("left_hand_v")==0){
            return 3;
        }else if(label.compare("right_hand")==0){
            return 4;
        }else if(label.compare("right_hand_ok")==0){
            return 5;
        }else if(label.compare("right_hand_v")==0){
            return 6;
        }else if(label.compare("face")==0){
            return 7;
        }
//    }
    return -1;
}

//把label 数字转换成文字结果，识别成功后，需要
string getLabelFromIndex(int predicated){
    if(predicated==1){
        return "left_hand";
    }else if(predicated==2){
        return "left_hand_ok";
    }else if(predicated==3){
        return "left_hand_v";
    }else if(predicated==4){
        return "right_hand";
    }else if(predicated==5){
        return "right_hand_ok";
    }else if(predicated==6){
        return "right_hand_v";
    }else if(predicated==7){
        return "face";
    }
    return "NULL";
}



int main()
{
    
    
    //加载机器学习数据
    Ptr<KNearest> knn= KNearest::create();
    knn->setDefaultK(3);
    knn->setIsClassifier(true);
    
    map<string,vector<vector<float>>> dataMap =getMLdata();
    map<string,vector<vector<float>>>::iterator it;
    

    
    it = dataMap.begin();
    long dataMatCol=it->second.at(0).size();//生成学习数据列
    int dataMatRow=0;
    
    vector<vector<float>> dataMatVectros;
    vector<int> labelMatVector;
    
    Mat dataMat;
   
    
    while(it != dataMap.end())
    {
        
        cout<<it->first<<" "<<it->second.size()<<" features "<<endl;
        
        dataMatRow+=it->second.size();
        
        vector<vector<float>> dataItems=it->second;
        
        for(size_t i=0,isize=dataItems.size();i<isize;i++){
            
            vector<float> dataItem=dataItems.at(i);
            
            Mat dataItemMat(1,(int)dataItem.size(),CV_32F);
            
            for(int j=0,jsize=(int)dataItem.size();j<jsize;j++){
                dataItemMat.at<float>(0, j)=dataItem.at(j);
            }
           
            dataMat.push_back(dataItemMat);
            
            labelMatVector.push_back(getLabelMatNumber(it->first));
            

        }
        it ++;
    }
    
    Mat labelMat;
    
    for(int j=0,jsize=(int)labelMatVector.size();j<jsize;j++){
        int label=labelMatVector.at(j);
//        cout<<"label:"<<label<<endl;
//        labelMat.at<int>(j, 1)=label;
        labelMat.push_back(label);
    }
    
//    cout<<labelMat<<endl;
    cout<<"data mat type:"<<dataMat.type()<<endl;
    
//    dataMat.convertTo(dataMat, CV_32F);
//    knn->train(dataMat);
    
    
    Ptr<TrainData> tData= TrainData::create(dataMat, ROW_SAMPLE, labelMat);
    knn->train(tData);
    
    cout<<"总的机器学习数据行:"<<dataMatRow<<" 列："<<dataMatCol<<endl;
//    cout<<labelMat<<endl;
//    cout<<dataMat<<endl;
    
    
    //打开摄像头开始识别图片
    
    VideoCapture cam(0);
    if(!cam.isOpened())
        return 0;
    
    
    
    
    double fps;
    char fpst[32];  // 用于存放帧率的字符串
    double t = 0;
    
    RNG rng(12345);
    
    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    
    HOGDescriptor *hog=new HOGDescriptor(Size(sacnWidnowSize,sacnWidnowSize),
                                         Size(sacnWidnowSize/2,sacnWidnowSize/2),
                                         Size(sacnWidnowSize/4,sacnWidnowSize/4),
                                         Size(sacnWidnowSize/4,sacnWidnowSize/4),9);
    

    
    while(true)
    {
        
        t = (double)cv::getTickCount();//帧率开始
        
        cam >> input_image;
        
        flip(input_image, input_image, 1);
        
        if(input_image.empty())
            return 0;
        
        
        
        output_mask = Mat::zeros(input_image.size(), CV_8UC1);
        
        Tool::getSkinOutput(input_image, output_mask);
        
        
        vector< vector<Point> > contours;   // 轮廓
        vector< vector<Point> > filterContours; // 筛选后的轮廓
        vector< Vec4i > hierarchy;    // 轮廓的结构信息
        contours.clear();
        hierarchy.clear();
        filterContours.clear();
        
        findContours(output_mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        
        vector<vector<Point>> hull(contours.size());
        // 去除伪轮廓
        for (size_t i = 0; i < contours.size(); i++)
        {
            
            Rect r=boundingRect(contours[i]);
            
            float whs=r.width*1.0f/r.height;
            float hws=r.height*1.0/r.width;
            
            long abs=fabs(contourArea(Mat(contours[i]))) ;
            
            int ow=output_mask.cols;
            int oh=output_mask.rows;
            
            
            if((r.width>100 && r.height>100)/*&&(whs>=0.6f && whs<=1.6f)*/&&(abs> 1000) && hws>0.9f ){
                
                filterContours.push_back(contours[i]);
                
                Tool::drawRectangle(output_image, r);
                Tool::drawRectangle(output_mask, r);
                Tool::drawRectangle(input_image,r);
                
                int resizeRect=240;
               
                Mat srcMat=output_mask(r);
                Mat newMat(resizeRect,resizeRect,srcMat.type());
                
                resize(srcMat, newMat, Size(resizeRect,resizeRect));//归一化，都变成240X240大小
                
                vector<float> tempdescripters;
                hog->compute(newMat, tempdescripters);
                Mat inputHogMat(1,(int)dataMatCol,CV_32F);
                
                for(int j=0,jsize=(int)tempdescripters.size();j<jsize;j++){
                    inputHogMat.at<float>(0, j)=tempdescripters.at(j);
                }
//                cout<<"input type:"<<inputHogMat.type()<<endl;
//                Mat result;
//                cout<<inputHogMat<<endl;
                float response=knn->predict(inputHogMat);
                string result=getLabelFromIndex( response);
//
                
                Tool::drawText(r.tl(), result, output_mask);
                 Tool::drawText(r.tl(), result, input_image);
                 Tool::drawText(r.tl(), result, output_image);
                
                cout<<"结果result:"<<" response: "<<result<<endl;
                
                
            }
            
        }
        
        output_mask.setTo(0);
        
        drawContours(output_mask, filterContours, -1, Scalar(255,0,0), CV_FILLED); //8, hierarchy);
        
        input_image.copyTo(output_image, output_mask);
        
        
        //====================
        t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
        fps = 1.0 / t;
        sprintf(fpst, "fps:%.2f", fps);      // 帧率保留两位小数
        //帧率结束
        
        Tool::drawText(Point(100,100), string(fpst), output_mask);
        
        
        imshow("input image msk", output_mask);
        imshow("input image", input_image);
        imshow("output image", output_image);
        
        output_image.setTo(0);
        if(27 == waitKey(30))
            return 0;
    }
    return 0;
}
