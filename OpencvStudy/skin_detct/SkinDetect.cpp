#include "headers.hpp"
#include "Tool.hpp"

Mat input_image;
Mat output_mask;
Mat output_image;
Mat mask;

int resizeRect=240;//图像大小（采集图像，处理特征的时候归一化大小）
int sacnWidnowSize=resizeRect;

//从数据文件夹中生成训练数据,这里的数据可以通过CreateMLdata.cpp采集后，人工区别后放到ml_data下对应的文件夹中
map<string, vector<vector<float>>> getMLdata(){
    
    //    int MAX_PATH=120;
    //    char   buffer[MAX_PATH];
    //    getcwd(buffer, MAX_PATH);
    //    cout<<string(buffer)<<endl;
    //
   //这是存放采样数据的地方，这里不方便使用使用执行文件的相对路径，所有换了文件夹，要修改project 下ml_data的路径
    string rootdir="/Users/wangzy/study/opencv/OpencvStudy/OpencvStudy/ml_data";
    
    vector<string> files=Tool::listFiles(rootdir);
    map<string, vector<vector<float>>> dataMap;
    
    //注意这里的HOG特征参数，扫描窗口越小，越准确，但是也越耗时
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
                
                resize(mat, newMat, Size(resizeRect,resizeRect));//归一化，都变成240X240大小后再采集特征
                
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
        
        dataMatRow+=it->second.size();//统计出所有的数据行
        
        vector<vector<float>> dataItems=it->second;
        
        for(size_t i=0,isize=dataItems.size();i<isize;i++){
            
            vector<float> dataItem=dataItems.at(i);//把每一个特征取出来，转换成Mat
            
            Mat dataItemMat(1,(int)dataItem.size(),CV_32F);
            
            for(int j=0,jsize=(int)dataItem.size();j<jsize;j++){//特征单个vector转换成 1行多列的Mat（没找到更好的办法，不过这个方法也没啥问题）
                dataItemMat.at<float>(0, j)=dataItem.at(j);
            }
           
            dataMat.push_back(dataItemMat);//把所有的单个Mat 组装成个一个大的训练集Mat
            
            labelMatVector.push_back(getLabelMatNumber(it->first));//所有的数据label
            

        }
        it ++;
    }
    
    Mat labelMat;
    for(int j=0,jsize=(int)labelMatVector.size();j<jsize;j++){//label的vector转Mat
        int label=labelMatVector.at(j);
        labelMat.push_back(label);
    }

    //设置训练数据
    Ptr<TrainData> tData= TrainData::create(dataMat, ROW_SAMPLE, labelMat);
    knn->train(tData);
    
    cout<<"总的机器学习数据行:"<<dataMatRow<<" 列："<<dataMatCol<<endl;

    
    //一切准备工作完成，打开摄像头开始识别图片
    
    VideoCapture cam(0);
    if(!cam.isOpened())
        return 0;
    
    
    double fps;
    char fpst[32];  // 用于存放帧率的字符串
    double t = 0;
    
    RNG rng(12345);
    
    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    
    //Hog特征采集器，用来采集摄像头图片区域的特征，这里的参数必须和CraeteMLData中采集数据时一样，不然数据特征纬度对不上，无法识别
    HOGDescriptor *hog=new HOGDescriptor(Size(sacnWidnowSize,sacnWidnowSize),
                                         Size(sacnWidnowSize/2,sacnWidnowSize/2),
                                         Size(sacnWidnowSize/4,sacnWidnowSize/4),
                                         Size(sacnWidnowSize/4,sacnWidnowSize/4),9);
    

    
    while(true)
    {
        
        t = (double)cv::getTickCount();//帧率开始
        cam >> input_image;
        
        flip(input_image, input_image, 1);//图片反转，MAC OS上opecv采集的图片是f左右相反的，这里特殊处理一下
        
        if(input_image.empty())
            return 0;
        
        
        //新建一个空Mat 存放皮肤区域数据
        output_mask = Mat::zeros(input_image.size(), CV_8UC1);
        //采集皮肤区域
        Tool::getSkinOutput(input_image, output_mask);
        
        
        vector< vector<Point> > contours;   // 轮廓
        vector< vector<Point> > filterContours; // 筛选后的轮廓
        vector< Vec4i > hierarchy;    // 轮廓的结构信息
        contours.clear();
        hierarchy.clear();
        filterContours.clear();
        
        //开始寻找轮廓
        findContours(output_mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        
        vector<vector<Point>> hull(contours.size());
        // 去除一些没有用的区域（比如太小，或者比例和手脸偏差太大的区域，这些区域大概率不是人体面部区域）
        for (size_t i = 0; i < contours.size(); i++)
        {
            
            //找到这些区域后，用矩形框标记出来
            Rect r=boundingRect(contours[i]);
            
            float whs=r.width*1.0f/r.height;//宽高比
            float hws=r.height*1.0/r.width;//高宽比，其实用一个比例就可以了
            
            //计算向量面积，也是一种排出法，排出更多的非肤色区域
            long abs=fabs(contourArea(Mat(contours[i]))) ;
            
            int ow=output_mask.cols;
            int oh=output_mask.rows;
            
            
            if((r.width>100 && r.height>100)/*&&(whs>=0.6f && whs<=1.6f)*/&&(abs> 1000) && hws>0.9f ){
                
                //把复合面部区域特征的区域保存下来
                filterContours.push_back(contours[i]);
                
                //并且把这些区域标记出来
                Tool::drawRectangle(output_image, r);
                Tool::drawRectangle(output_mask, r);
                Tool::drawRectangle(input_image,r);
                
               
                //新建一个resizeRect大小的区域，用来保存符合肤色区域数据，并且要把这块区域全部resize 采样时候的大小
                //就是resizeRect的大小，方便取得一样维度的特征，而且并不会影响识别结果（为什么不会，这里自己要想清楚）
                Mat srcMat=output_mask(r);
                Mat newMat(resizeRect,resizeRect,srcMat.type());
                
                resize(srcMat, newMat, Size(resizeRect,resizeRect));//归一化，都变成240X240大小
                
                vector<float> tempdescripters;//采集的特征放到了这里
                hog->compute(newMat, tempdescripters);//采集这块图片的特征
                Mat inputHogMat(1,(int)dataMatCol,CV_32F);
                //用这个for循环把特征vector变成Mat
                for(int j=0,jsize=(int)tempdescripters.size();j<jsize;j++){
                    inputHogMat.at<float>(0, j)=tempdescripters.at(j);
                }

                //识别结果，因为是KNN所有无论如何总会得到一个结果，这个结果必然是LabelMat中的一个
                float response=knn->predict(inputHogMat);//得到的结果必然是LablMat中的一个
                string result=getLabelFromIndex( response);//对应成文字
                
                //在多种窗口中绘制出来
                Tool::drawText(r.tl(), result, output_mask);
                Tool::drawText(r.tl(), result, input_image);
                Tool::drawText(r.tl(), result, output_image);
                
                cout<<"结果result:"<<" response: "<<result<<endl;
                
            }
            
        }
        
        output_mask.setTo(0);//使用前重置一下
        drawContours(output_mask, filterContours, -1, Scalar(255,0,0), CV_FILLED); //8, hierarchy);//把肤色区域绘制进去
        input_image.copyTo(output_image, output_mask);//成功把肤色区域拷贝过去
        
        
        //====================
        t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
        fps = 1.0 / t;
        sprintf(fpst, "fps:%.2f", fps);      // 帧率保留两位小数
        //帧率结束
        //以下是绘制帧率，目前帧率比较少，这个demo自然还有很多优化空间
        Tool::drawText(Point(100,100), string(fpst), output_mask);
        Tool::drawText(Point(100,100), string(fpst), input_image);
        Tool::drawText(Point(100,100), string(fpst), output_image);
        //窗口太多会y影响帧率
//        imshow("input image msk", output_mask);//Mask窗口
        imshow("input image", input_image);//原始图
//        imshow("output image", output_image);//输出图
        
        output_image.setTo(0);
        
        if(27 == waitKey(30)){
            delete hog;
            return 0;
        }
        
    }
    return 0;
}
