/*
 * @Author: your name
 * @Date: 2021-09-03 16:56:37
 * @LastEditTime: 2021-11-23 14:31:44
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /opencv_input_file/main.cpp
 */

#include <string>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <dirent.h>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"


using namespace cv;
using namespace std;

typedef struct _AI_BODY_INFO{
    float confidence;
    int type;
    int x;
    int y;
    int w;
    int h;
}BODY_INFO;

typedef struct _AI_FACE_INFO{
    int x;
    int y;
    int w;
    int h;
    int eye_status;
    int theta_left;
    int theta_right;
}FACE_INFO;

typedef struct _AI_BODY_INFOS{
    int frame_id;
    std::string ai_name;
    std::vector<BODY_INFO> vec_bodys;

}AI_BODY_INFO;


typedef struct _AI_FACE_INFOS{
    int frame_id;
    std::string ai_name;
    std::vector<FACE_INFO> vec_faces;

}AI_FACE_INFO;

typedef struct _EVENT_RECORDS{
    int frame_id;
    int event_type;
}event_records;

bool GetBodyAIResults(string filename, std::vector<AI_BODY_INFO>& vec_body_results)
{
    // std::vector<AI_INFO> vec_ai_results;
    string path_ = "./"+filename+"_body_results.txt";
    std::ifstream ifile_;
    ifile_.open(path_.c_str());
    if (!ifile_.is_open()){
        printf("fail to open file ...%s\n", path_.c_str());
    }        
    
    std::stringstream strStream;
    std::string strTemp;

    vector<string> vec_str;    

    while (ifile_.good() && !ifile_.eof())
    {
        getline(ifile_, strTemp, '\n');
        strStream.clear();
        strStream.str(strTemp);        
        // cout<<"strStream "<<strStream.str()<<endl;
        if (strStream.fail()){
            continue;
        }
        string s;
        istringstream iss(strStream.str());
        vec_str.clear();
        while(iss>>s){
            vec_str.push_back(s);
            // cout<<s<<" ";
        }
        // cout<<endl;
        // cout<<"vec_str size is "<<vec_str.size()<<endl;

        AI_BODY_INFO ai_info;
        BODY_INFO single_body_info;
        ai_info.frame_id = std::stoi(vec_str[0]);
        ai_info.vec_bodys.clear();
        
        if(vec_str.size()> 1){
            if(vec_str[1].compare(std::string("Body")) == 0){
                
                ai_info.ai_name = vec_str[1];
                single_body_info.confidence = std::stof(vec_str[2]);
                // cout<<"confidence : "<<single_body_info.confidence<<endl;
                single_body_info.type = std::stoi(vec_str[3]);
                single_body_info.x = std::stoi(vec_str[4]);
                single_body_info.y = std::stoi(vec_str[5]);
                single_body_info.w = std::stoi(vec_str[6]);
                single_body_info.h = std::stoi(vec_str[7]);                
                ai_info.vec_bodys.push_back(single_body_info);

                if(vec_str.size() > 8 && vec_str[8].compare(std::string("Body")) == 0){
                    single_body_info.confidence = std::stof(vec_str[9]);
                    // cout<<"confidence : "<<single_body_info.confidence<<endl;
                    single_body_info.type = std::stoi(vec_str[10]);
                    single_body_info.x = std::stoi(vec_str[11]);
                    single_body_info.y = std::stoi(vec_str[12]);
                    single_body_info.w = std::stoi(vec_str[13]);
                    single_body_info.h = std::stoi(vec_str[14]);
                    ai_info.vec_bodys.push_back(single_body_info);
                }
            }
        }
        vec_body_results.push_back(ai_info);
    }
    ifile_.close();

    return true;
}

bool GetFaceAIResults(string filename, std::vector<AI_FACE_INFO>& vec_face_results)
{

    string path_ = "./"+filename+"_face_results.txt";
    std::ifstream ifile_;
    ifile_.open(path_.c_str());
    if (!ifile_.is_open()){
        printf("fail to open file ...%s\n", path_.c_str());
    }        
    
    std::stringstream strStream;
    std::string strTemp;

    vector<string> vec_str;    

    while (ifile_.good() && !ifile_.eof())
    {
        getline(ifile_, strTemp, '\n');
        strStream.clear();
        strStream.str(strTemp);        
        // cout<<"strStream "<<strStream.str()<<endl;
        if (strStream.fail()){
            continue;
        }
        string s;
        istringstream iss(strStream.str());
        vec_str.clear();
        while(iss>>s){
            vec_str.push_back(s);
            // cout<<s<<" ";
        }
        // cout<<endl;
        // cout<<"vec_str size is "<<vec_str.size()<<endl;

        AI_FACE_INFO ai_info;
        FACE_INFO single_face_info;
        ai_info.frame_id = std::stoi(vec_str[0]);
        ai_info.vec_faces.clear();
        
        if(vec_str.size()> 1){
            if(vec_str[1].compare(std::string("Face")) == 0 ){
                ai_info.ai_name = vec_str[1];
                single_face_info.x = std::stoi(vec_str[2]);
                single_face_info.y = std::stoi(vec_str[3]);
                single_face_info.w = std::stoi(vec_str[4]);
                single_face_info.h = std::stoi(vec_str[5]);
                if(vec_str.size() > 6 && vec_str[6].compare(std::string("Face")) != 0 ){
                    single_face_info.eye_status = std::stoi(vec_str[6]);
                    single_face_info.theta_left = std::stoi(vec_str[7]);
                    single_face_info.theta_right = std::stoi(vec_str[8]);
                    ai_info.vec_faces.push_back(single_face_info);
                    if(vec_str.size() > 9 && vec_str[9].compare(std::string("Face")) == 0 ){
                        single_face_info.x = std::stoi(vec_str[10]);
                        single_face_info.y = std::stoi(vec_str[11]);
                        single_face_info.w = std::stoi(vec_str[12]);
                        single_face_info.h = std::stoi(vec_str[13]);
                        if(vec_str.size() > 14){
                            single_face_info.eye_status = std::stoi(vec_str[14]);
                            single_face_info.theta_left = std::stoi(vec_str[15]);
                            single_face_info.theta_right = std::stoi(vec_str[16]);
                        }else{
                            single_face_info.eye_status = -1;
                            single_face_info.theta_left = -1;
                            single_face_info.theta_right = -1;
                        }
                        ai_info.vec_faces.push_back(single_face_info);
                    }
                }else if(vec_str.size() > 6 && vec_str[6].compare(std::string("Face")) == 0 ){
                    ai_info.vec_faces.push_back(single_face_info);
                    single_face_info.x = std::stoi(vec_str[7]);
                    single_face_info.y = std::stoi(vec_str[8]);
                    single_face_info.w = std::stoi(vec_str[9]);
                    single_face_info.h = std::stoi(vec_str[10]);
                    single_face_info.eye_status = std::stoi(vec_str[11]);
                    single_face_info.theta_left = std::stoi(vec_str[12]);
                    single_face_info.theta_right = std::stoi(vec_str[13]);
                    ai_info.vec_faces.push_back(single_face_info);
                }else{
                    single_face_info.eye_status = -1;
                    single_face_info.theta_left = -1;
                    single_face_info.theta_right = -1;
                    ai_info.vec_faces.push_back(single_face_info);
                }
            }
        }
        vec_face_results.push_back(ai_info);
    }
    ifile_.close();

    return true;
}

bool GetEventAIResults(string filename, std::vector<event_records>& er_vec)
{
    std::ifstream ifile_;
    string path_ = "./"+filename+"_event_records.txt";
    ifile_.open(path_.c_str());
    if (!ifile_.is_open()){
        printf("fail to open file ...%s\n", path_.c_str());
    }        
    
    std::stringstream strStream;
    std::string strTemp;
    int frame_id;
    int type = 0;
    while (ifile_.good() && !ifile_.eof())
    {
        getline(ifile_, strTemp, '\n');
        strStream.clear();
        strStream.str(strTemp);
        cout<<"strStream "<<strStream.str()<<endl;

        strStream >> frame_id>> type ;
        if (strStream.fail()){
            continue;
        }
        event_records er;
        er.frame_id = frame_id;
        er.event_type = type;
        er_vec.push_back(er);
    }
    ifile_.close();


    return true;
}


void mkdir_path(const char *p_cMkdir)
{
    int isCreate = mkdir(p_cMkdir,S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
 
    if( !isCreate )
    printf("create path:%s\n",p_cMkdir);
    else
    printf("create path failed! error code : %s \n",p_cMkdir);

}

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("usage: ./bin dir\n");
        return -1;
    }
    
    std::string input_dir = std::string(argv[1]);
    // std::vector<std::string> files = GetListFiles(input_dir);
    std::vector<AI_BODY_INFO> vec_body_results;
    GetBodyAIResults(input_dir, vec_body_results);
    for(auto var:vec_body_results){
        cout<<"frame_id: "<<var.frame_id<<" ";
        cout<<"ai_name: "<<var.ai_name<<" ";
        for(auto body:var.vec_bodys){
            cout<<"confidence: "<<body.confidence<<" ";
            cout<<"type: "<<body.type<<" ";
            cout<<"x: "<<body.x<<" ";
            cout<<"y: "<<body.y<<" ";
            cout<<"w: "<<body.w<<" ";
            cout<<"h: "<<body.h<<" ";
        }
        cout<<" Done"<<endl;
    }

    cout<<"-------------------------------------------------------"<<endl;

    std::vector<AI_FACE_INFO> vec_face_results;
    GetFaceAIResults(input_dir, vec_face_results);

    for(auto var:vec_face_results){
        cout<<"frame_id: "<<var.frame_id<<" ";
        cout<<"ai_name: "<<var.ai_name<<" ";
        for(auto face:var.vec_faces){
            cout<<"eye_status: "<<face.eye_status<<" ";
            cout<<"theta left: "<<face.theta_left<<" ";
            cout<<"theta right: "<<face.theta_right<<" ";
            cout<<"x: "<<face.x<<" ";
            cout<<"y: "<<face.y<<" ";
            cout<<"w: "<<face.w<<" ";
            cout<<"h: "<<face.h<<" ";
        }
        cout<<" Done"<<endl;
    }

    std::vector<event_records> er_vec;
    GetEventAIResults(input_dir, er_vec);
    for(auto er:er_vec){
        std::cout << er.frame_id << " " << er.event_type << " " << std::endl;
    }
    // return 0;
    
    printf("argc = %d, input_dir = %s\n", argc, input_dir.c_str());
    // VideoCapture capture;
    cv::Mat frame;
    int all_count = 0;
    int file_count = 0;
    int body_index = 0;
    int face_index = 0;
    bool body_flag = false;
    bool face_flag = false;

    string output_dir = std::string("./") + input_dir +std::string("_ai/");
    if (access(output_dir.c_str(), F_OK) != 0) {
        mkdir_path(output_dir.c_str());
    }

    while(true){

        string file_name = "/frame-"+std::to_string(file_count)+".jpg";
        string specif_file = input_dir + file_name;
        printf("specific file: %s\n", specif_file.c_str());     
        
        string output_file = output_dir + file_name;

        cv::Mat _frame = cv::imread(specif_file);
        if(!_frame.data) {
            printf("all count image is %d, exit\n", all_count);
            break;
        }

        for(size_t i=0; i<vec_body_results.size()-1; i++){
            
            if(file_count >= vec_body_results[i].frame_id && file_count < vec_body_results[i+1].frame_id 
                && vec_body_results[i].ai_name.compare(std::string("Body")) == 0 ){

                body_index = i;
                body_flag = true;
                // printf("info = %d, %d, %d, %d\n", body_index, file_count, vec_body_results[i].frame_id, vec_body_results[i+1].frame_id);
                break;
            }else{
                body_flag = false;
            }
        }

        for(size_t i=0; i<vec_face_results.size()-1; i++){
            
            if(file_count >= vec_face_results[i].frame_id && file_count < vec_face_results[i+1].frame_id 
                && vec_face_results[i].ai_name.compare(std::string("Face")) == 0 ){

                face_index = i;
                face_flag = true;
                // printf("info = %d, %d, %d, %d\n", face_index, file_count, vec_face_results[i].frame_id, vec_face_results[i+1].frame_id);
                break;
            }else{
                face_flag = false;
            }
        }
        
        printf("body_index == %d, face_index == %d\n", body_index, face_index);

        if(body_flag){
            for(auto body:vec_body_results[body_index].vec_bodys){
                if(body.type == 1){
                    string sconfidence = std::to_string(body.confidence);
                    cv::rectangle(_frame, cv::Rect(body.x, body.y, body.w, body.h), cv::Scalar(255, 0, 0), 2);
                    cv::putText(_frame, sconfidence, cv::Point(body.x,body.y+12), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0));
                    
                    // printf("body rectangle -> frame: %d %d %d %d\n", body.x, body.y, body.w, body.h);
                }else if(body.type == 2){
                    cv::rectangle(_frame, cv::Rect(body.x, body.y, body.w, body.h), cv::Scalar(0, 255, 0), 2);
                }
            } 
        }
        
        if(face_flag){
            for(auto face:vec_face_results[face_index].vec_faces){
                cv::Scalar color;
                if(face.eye_status == 1){
                    color =cv::Scalar(0,0,255);
                    cv::putText(_frame, std::string("close"), cv::Point(face.x, face.y+10), cv::FONT_HERSHEY_SIMPLEX, 0.5, color);
                    
                }else if(face.eye_status == 0){
                    color =cv::Scalar(0,255,0);
                    cv::putText(_frame, std::string("open"), cv::Point(face.x, face.y+10), cv::FONT_HERSHEY_SIMPLEX, 0.5, color);
                }else{
                    color =cv::Scalar(255,0,0);
                }
                if(face.theta_left > 0 && face.theta_right > 0){
                    char text1[256];
                    sprintf(text1, "thetaL: %d , thetaR: %d", face.theta_left, face.theta_right);
                    cv::putText(_frame, std::string(text1), cv::Point(face.x, face.y), cv::FONT_HERSHEY_SIMPLEX, 0.5, color);
                }
                cv::rectangle(_frame, cv::Rect(face.x, face.y, face.w, face.h), color, 1.5);
                // printf("face rectangle -> frame: %d %d %d %d\n", face.x, face.y, face.w, face.h);
            }
        }

        for(auto event_record:er_vec){
            int display_event = file_count - event_record.frame_id;
            if(display_event >= 0 && display_event < 25 ){
                int event_type = event_record.event_type;
                if(event_type == 1){
                    cv::putText(_frame, "OUT DANGER!", cv::Point(10,340),cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 255));
                }else if(event_type == 2){
                    cv::putText(_frame, "WAKEUP!", cv::Point(80,340),cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255));    
                }else if(event_type == 3){
                    cv::putText(_frame, "SLEEP!", cv::Point(80,340),cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255)); 
                }                
            }
        }

        cv::rectangle(_frame, cv::Rect(64, 36,(640*0.9-64), (360*0.9-36)), cv::Scalar(0, 255, 255), 1);


        cv::imwrite(output_file, _frame);
        file_count++;
        all_count ++;

        waitKey(10);
    }

    printf("all count image is %d, exit\n", all_count);

    return 0;
}