


#include "Tool.hpp"
#include "ImageTool.hpp"

namespace NumberRec {


    Mat dataMat;//所有的样本数据
    Mat labelMat;//样本所代表的数字


    Ptr<KNearest> knn;

    void init(){



        //=======================第一步开始=================================


        string dataRoot="/Users/wangzy/study/opencv/OpencvStudy/OpencvStudy/number_data/";

        cout<<"注意你的样本文件保存路径,当前路路经视作者的电脑路经"<<endl;

        cout<<"第一步:采集数据==========================="<<endl;

        vector<string> fs=Tool::listFiles(dataRoot);



        char fn[1024];

        for(int i=0;i<10;i++){

            sprintf(fn, "%s/%d/",dataRoot.c_str(),i);
            //        cout<<fn<<endl;
            vector<string> datas=Tool::listFiles(fn);


            for(string data:datas){

                string absPath=string(fn)+data;
                vector<float> hogs=ImageTool::getHog(absPath);

                Mat tempMat(1,(int)hogs.size(),CV_32F);

                for(int i=0,isize=(int)hogs.size();i<isize;i++){
                    tempMat.at<float>(0, i)=hogs.at(i);
                }
                labelMat.push_back(i);

                dataMat.push_back(tempMat);

            }
        }

        //=======================第一步结束=================================
        //=======================第二步开始=================================


        cout<<"第二步:调用Opencv API 组装数据==========================="<<endl;

        knn = KNearest::create();  //创建knn分类器
        knn->setDefaultK(5);//设定k值
        knn->setIsClassifier(true);
        Ptr<TrainData> tData = TrainData::create(dataMat, ROW_SAMPLE, labelMat);



        knn->train(tData);
        //=======================第二步结束=================================

        //=======================第三步开始=================================



    }

    int predicatImg(Mat inputMat) {

        if(dataMat.rows==0){

            cout<<"调用识别之前，请先调用init初始化"<<endl;
            return -1;
        }


        Mat mat= ImageTool::getHogFromMat(inputMat);


        float response = knn->predict(mat);


        return response;


    }

}
