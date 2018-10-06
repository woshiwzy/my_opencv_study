

namespace ImageTool {
    
 
    
    
    vector<float> getHog(string path){
        
        
        
        Mat imageMat;
        imageMat = imread(path, 1);//
        
        
        
        HOGDescriptor *hog = new HOGDescriptor(cvSize(128, 128), cvSize(64, 64), cvSize(32, 32), cvSize(32, 32), 9);
        vector<float> descriptors;
        hog->compute(imageMat, descriptors);
        
        
        return descriptors;
    }
    
    
    Mat getHotMat(string path){
        
        vector<float> mats=getHog(path);
        
        Mat ret(1, (int)mats.size(), CV_32F);
        
        
        for(int i=0,isize=(int)mats.size();i<isize;i++){
            
            ret.at<float>(0, i)=mats.at(i);
        }
        
        return ret;
    }
    
    
    Mat getHogFromMat(Mat inputMat){
        
        
        
        HOGDescriptor *hog = new HOGDescriptor(cvSize(128, 128), cvSize(64, 64), cvSize(32, 32), cvSize(32, 32), 9);
        vector<float> descriptors;
        hog->compute(inputMat, descriptors);
        
        Mat ret(1, (int)descriptors.size(), CV_32F);
        
        
        for(int i=0,isize=(int)descriptors.size();i<isize;i++){
            
            ret.at<float>(0, i)=descriptors.at(i);
        }
        
        return ret;
        
       
    }

    
}








