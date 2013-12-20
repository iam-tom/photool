#include<boost/filesystem.hpp>
#include<boost/filesystem/operations.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>
#include<map>
#include<ctime>


bool get_img_list(boost::filesystem::path& path,std::vector<boost::filesystem::path>& img_list);

bool get_img_list(boost::filesystem::path& path,std::vector<std::string> filetypes,std::vector<boost::filesystem::path>&img_list);

