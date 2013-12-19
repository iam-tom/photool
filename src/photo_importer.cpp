#include"photo_utils.h"
#include<boost/filesystem.hpp>
#include <boost/progress.hpp>

bool make_folfers(boost::filesystem::path& base_folder)
{

  boost::filesystem::path raw_folder=base_folder/"raw";
  boost::filesystem::path jpg_folder=base_folder/"jpg";
  boost::filesystem::path snapshot_folder=base_folder/"snapshot";


  if(boost::filesystem::is_directory(base_folder))
      {
        if(!boost::filesystem::is_directory(raw_folder))boost::filesystem::create_directory(raw_folder);
        if(!boost::filesystem::is_directory(jpg_folder))boost::filesystem::create_directory(jpg_folder);
        if(!boost::filesystem::is_directory(snapshot_folder))boost::filesystem::create_directory(snapshot_folder);

      }


}
int main (int argc,const char* argv[])
{
  bool vis=true;
  boost::progress_display * progress_bar = NULL;

  boost::filesystem::path import_dir(argv[1]);
  boost::filesystem::path target_dir(argv[2]);

  std::cout<<"importing from :  "<<import_dir<<std::endl;
  std::cout<<"importing to   :  "<<target_dir<<std::endl;

  std::vector<std::string> img_list;
  get_img_list(import_dir,img_list);




  if(vis)progress_bar = new boost::progress_display(img_list.size());
  for(int i=0;i<img_list.size();++i)
  {



    //progress
    ++(*progress_bar);
    if(vis)boost::progress_display show_progress();
  }
make_folfers(target_dir);

}
