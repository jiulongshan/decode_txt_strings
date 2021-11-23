/*
 * @Author: your name
 * @Date: 2021-11-04 09:45:01
 * @LastEditTime: 2021-11-04 14:59:46
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /opencv_input_file/stringstream.cpp
 */
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <iostream>


using namespace std;

int main(int argc, char* argv[])
{
    string path = "./event_records.txt";
    ofstream ofile;//(path.c_str(), ios::beg);
    ofile.open(path.c_str());
    if (!ofile.is_open())
        printf("fail to open file ...\n");
    string data = "33 1\n";
    ofile.seekp(0, ios::end);
    ofile << data;
    // ofile.close();
    //2020-11-03 09:30:40 2\n2021-11-03 09:30:50 3";
    // ofile.open(path.c_str(), ios_base::app);
    // if (!ofile.is_open())
        // printf("fail to open file ...\n");
    // ofile.seekp(0,ios::end);
    ofile.seekp(0, ios::end);
    data = "161 2\n";
    ofile << data;
    // ofile.close();

    // ofile.open(path.c_str(), ios_base::app);
    // if (!ofile.is_open())
        // printf("fail to open file ...\n");
    // ofile.seekp(0,ios::end);
    ofile.seekp(0, ios::end);
    data = "444 3\n";
    ofile << data;
    ofile.close();
    ////////////////////////////////////////////////

    std::ifstream ifile("./event_records.txt");
    if (!ifile.is_open()) {
        printf("fail to open file ...\n");
    }
    stringstream ss;
    ss << ifile.rdbuf();
    ifile.close();
    string filedata = ss.str();
    cout<<"file data is: "<<endl<<filedata<<endl;

    
    std::ifstream ifsAddr("event_records.txt");
    if (!ifsAddr.is_open()) {
        printf("fail to open file ...\n");
    }

    std::string strTemp;
    std::string strName;
    int frame_id;
    int type = 0;
    std::stringstream strStream;
    int count = 0;
    while (ifsAddr.good() && !ifsAddr.eof())
    {
        getline(ifsAddr, strTemp, '\n');
        strStream.clear();
        strStream.str(strTemp);
        cout<<"strStream "<<strStream.str()<<endl;

        strStream >> frame_id>> type ;
        if (strStream.fail()){
            continue;
        }            
        count++;
        std::cout << frame_id << " " << type << " " << std::endl;
        cout<<count<<endl;
    }
    
    return 0;
}