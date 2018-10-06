////获取机器学习数据结果特征
//
//#include "headers.hpp"
//#include "Tool.hpp"
//
////获取机器学习数据结果特征，可以把这个数据存到数据库
//map<string, vector<vector<float>>> getMLdata(){
//    
//    //    int MAX_PATH=120;
//    //    char   buffer[MAX_PATH];
//    //    getcwd(buffer, MAX_PATH);
//    //    cout<<string(buffer)<<endl;
//    //
//    int resizeRect=240;
//    string rootdir="/Users/wangzy/workspace/opencv_work/OpencvStudy/OpencvStudy/ml_data";
//    
//    vector<string> files=Tool::listFiles(rootdir);
//    map<string, vector<vector<float>>> dataMap;
//    int sacnWidnowSize=resizeRect;
//    
//     HOGDescriptor *hog=new HOGDescriptor(Size(sacnWidnowSize,sacnWidnowSize),
//                                          Size(sacnWidnowSize/2,sacnWidnowSize/2),
//                                          Size(sacnWidnowSize/4,sacnWidnowSize/4),
//                                          Size(sacnWidnowSize/4,sacnWidnowSize/4),9);
//    
//    for(size_t i=0,isize=files.size();i<isize;i++){
//        string file=files.at(i);
//        string absPath=string(rootdir+"/"+file);
//        
//        cout<<absPath<<endl;
//        
//        vector<vector<float>> descriptersVector;
//        
//        vector<string> subFiles=Tool::listFiles(absPath);
//        if(!subFiles.empty()){
//            cout<<"subfile count:"<<subFiles.size()<<endl;
//            
//            for(size_t j=0,jsize=subFiles.size();j<jsize;j++){
//                string imgAbsPath=absPath+"/"+subFiles.at(j);
//                Mat mat=imread(imgAbsPath);
//                //             cout<<imgAbsPath<<" "<<mat.rows<<","<<mat.cols;
//                Mat newMat(resizeRect,resizeRect,CV_8UC1);
//                
//                resize(mat, newMat, Size(resizeRect,resizeRect));//归一化，都变成240X240大小
//
//                vector<float> descripters;
//                hog->compute(newMat, descripters);
//                
//                descriptersVector.push_back(descripters);
//
//            }
//            
//            dataMap[file]=descriptersVector;//存放到Map 中去
//
//            
//        }
//    }
//    
//    delete hog;
//    return dataMap;
//}
//
//int main()
//{
//
//
//    //    cout<<"done"<<endl;
//
//   map<string,vector<vector<float>>> dataMap=getMLdata();
//
//   map<string,vector<vector<float>>>::iterator it;
//
//    it = dataMap.begin();
//
//    while(it != dataMap.end())
//    {
//        //it->first;
//        cout<<it->first<<" "<<it->second.size()<<" features "<<endl;
//
//        it ++;
//    }
//
//
//
//    return 0;
//}
