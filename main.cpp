/*
 * @Author: your name
 * @Date: 2021-09-03 16:56:37
 * @LastEditTime: 2021-11-18 19:44:37
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /opencv_input_file/main.cpp
 */

#include <string>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>

#include <dirent.h>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"


using namespace cv;
using namespace std;


void mkdir_path(const char *p_cMkdir)
{
    int isCreate = mkdir(p_cMkdir,S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
 
    if( !isCreate )
    printf("create path:%s\n",p_cMkdir);
    else
    printf("create path failed! error code : %s \n",p_cMkdir);

}

// 遍历指定文件夹下的所有文件，不包括指定文件夹内的文件夹
std::vector<std::string> GetListFiles(const std::string& path, const std::string& exten = "*")
{
    std::vector<std::string> list;
    list.clear();
 
    DIR* dp = nullptr;
    struct dirent* dirp = nullptr;
    if ((dp = opendir(path.c_str())) == nullptr) {
        return list;
    }
 
    while ((dirp = readdir(dp)) != nullptr) {
        if (dirp->d_type == DT_REG) {
            if (exten.compare("*") == 0){
                list.emplace_back(static_cast<std::string>(dirp->d_name));
            }else{
                if (std::string(dirp->d_name).find(exten) != std::string::npos)
                    list.emplace_back(static_cast<std::string>(dirp->d_name));
            }
        }
    }
 
    closedir(dp);
 
    return list;
}

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("usage: ./bin dir\n");
        return -1;
    }
    
    std::string input_dir = std::string(argv[1]);

    std::vector<std::string> files = GetListFiles(input_dir);
    
    
    printf("argc = %d, input_dir = %s\n", argc, input_dir.c_str());
    VideoCapture capture;
    cv::Mat frame;
    // frame= capture.open("/home/lhb/T31/videos/1630146163.mp4");
    // frame= capture.open("/home/lhb/T31/videos/0020210831232914.mp4");
    // frame= capture.open("/mnt/hgfs/work/Babymonitor/Agora/03.mp4");

    int all_count = 0;

    for(auto spefile:files){
        printf("specific file: %s\n", spefile.c_str());
        string specif_dir = input_dir + spefile + std::string("_dir");
        mkdir_path(specif_dir.c_str());
        printf("specif_dir : %s\n", specif_dir.c_str());

        string specif_file = input_dir + "/" +spefile;
        printf("specific file: %s\n", specif_file.c_str());
        capture.open(specif_file.c_str());
        if(!capture.isOpened())
        {
            printf("can not open ...\n");
            break;
        }
        // namedWindow("output", CV_WINDOW_AUTOSIZE);
        
        int count=0;
        char filename[32];
        int interval = 25;
        int inter_count = 0;
        // sprintf(filename, "frame-%d", count);
        while (capture.read(frame))
        {
            // imshow("output", frame);
            resize(frame, frame, Size(640, 360), CV_INTER_LINEAR);
            inter_count ++;
            // if(inter_count == interval){
                inter_count = 0;
                sprintf(filename, "%s/frame-%d.jpg", specif_dir.c_str(), count);
                cv::rectangle(frame, cv::Rect(64, 36,(640*0.9-64), (360*0.9-36)), cv::Scalar(0, 255, 255), 1);
                cv::imwrite(std::string(filename), frame);
                count++;
                all_count ++;
            // }
            
            // printf("writing .........................\n");
            waitKey(10);
        }
        
        capture.release();
    }

    printf("all count image is %d\n", all_count);

    return 0;
}