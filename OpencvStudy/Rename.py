import os

if __name__ == '__main__':
#    rootDir = "/Users/wangzy/study/opencv/OpencvStudy/OpencvStudy/ml_data"
    rootDir = "/Users/wangzy/study/opencv/OpencvStudy/OpencvStudy/number_data"
    files = os.listdir(rootDir)
    total = 0
    allfiles = []
    for f in files:
        if not f.startswith("."):
            absf = rootDir + os.sep + f
            print absf
            subfiels = os.listdir(absf)
            for subf in subfiels:
                if not subf.startswith("."):
                    subabs = absf + os.sep + subf
                    total = total + 1
                    allfiles.append(subabs)

    print "---------------"
    print total

    for x in range(0, total):
        absPath = allfiles[x]
        name = os.path.basename(file(absPath).name)
        path = os.path.dirname(absPath)

        print path, name,str(x)+".png"
        os.rename(absPath,path+os.sep+str(x)+".png")
