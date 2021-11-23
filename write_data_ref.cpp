//写输入参考代码：

void write()
{
	path = "./"+filename+"_body_results.txt";
    body_results_file_.open(path.c_str());
    if (!body_results_file_.is_open())
        printf("fail to open file ...%s\n", path.c_str());
	//face_results_file_.close();
	
	std::stringstream ss;
	string data;
	body_results_file_.seekp(0, ios::end);
	ss<<playback_count_;
	body_results_file_<<ss.str();
	
	char cdata[256];
	data = std::string(" Body ");
	body_results_file_<<data;
	sprintf(cdata, "%f %d %d %d %d %d", result.confidence, 2, (int)result.xmin, (int)result.ymin, (int)(result.xmax-result.xmin), (int)(result.ymax-result.ymin));
	body_results_file_<<std::string(cdata);
	
	data=std::string("\n");
	body_results_file_<<data;
	body_results_file_.flush();
}