#include"photo_utils.h"
#include<boost/filesystem.hpp>

int main(int argc,char* argv[])
{
  boost::filesystem::path base_path(argv[1]);
  int filter_val=atoi(argv[2]);

  boost::filesystem::path tag_file=base_path/"snapshot"/"tags.xml";
  boost::filesystem::path path_snapshots=base_path/"snapshot";
  boost::filesystem::path path_raw=base_path/"raw";
  boost::filesystem::path path_jpg=base_path/"jpg";
  boost::filesystem::path path_dev=base_path/"dev";

  if(!boost::filesystem::is_regular(tag_file))
  {
    std::cout<<"no tag file -aborting"<<std::endl;
    return 0;
  }

  std::vector<boost::filesystem::path> img_list;
  get_img_list(path_snapshots, img_list);

cv::FileStorage fs(tag_file.c_str(), cv::FileStorage::READ);

int num_images=img_list.size();
std::vector<int> rating_list;
rating_list.resize(num_images);

for(int i=0;i<num_images;i++)
{
std::string file=img_list[i].stem().c_str();
int rating=fs[file]["rating"];
rating_list[i]=rating;
boost::filesystem::path src_path_raw,src_path_jpg,dst_path_raw,dst_path_jpg;
std::cout<<"rating is="<<rating<<std::endl;
if(rating>=filter_val)
{
  std::string file_jpg=file+".JPG";
  std::string file_raw=file+".CR2";
  src_path_raw=path_raw/file_raw;
  src_path_jpg=path_jpg/file_jpg;

  dst_path_raw=path_dev/file_raw;
  dst_path_jpg=path_dev/file_jpg;

  if(boost::filesystem::is_regular(src_path_jpg) && !boost::filesystem::is_regular(dst_path_jpg)) boost::filesystem::copy_file(src_path_jpg,dst_path_jpg);
  if(boost::filesystem::is_regular(src_path_jpg) && !boost::filesystem::is_regular(dst_path_raw)) boost::filesystem::copy_file(src_path_raw,dst_path_raw);
}
}
fs.release();




}
