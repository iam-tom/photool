#include<photo_utils.h>

bool get_img_list(boost::filesystem::path& path,std::vector<boost::filesystem::path>& img_list)
{
  std::vector<std::string>supported_filetypes;
  // known filetypes
  supported_filetypes.push_back(".JPG");
  supported_filetypes.push_back(".jpg");
  supported_filetypes.push_back(".PNG");
  supported_filetypes.push_back(".png");
  supported_filetypes.push_back(".png");
  supported_filetypes.push_back(".png");
  supported_filetypes.push_back(".CR2");

  get_img_list(path,supported_filetypes,img_list);
}

bool get_img_list(boost::filesystem::path& path,std::vector<std::string> supported_filetypes,std::vector<boost::filesystem::path>&img_list)
{
  std::vector<boost::filesystem::path> input_files;
  if(boost::filesystem::is_directory(path))
  {
  copy(boost::filesystem::directory_iterator(path), boost::filesystem::directory_iterator(), std::back_inserter(input_files));
  }

//  std::vector<boost::filesystem::path> input_files;
//   std::map<boost::posix_time::ptime, boost::filesystem::path> files;
//   for (boost::filesystem::directory_iterator iter(path); iter != boost::filesystem::directory_iterator();
//   ++iter)
//   {
//      const boost::filesystem::path& cur_path = iter->path();
//      if (boost::filesystem::is_regular_file(cur_path))
//      {
//         boost::system::error_code error;
//         std::time_t last_write = boost::filesystem::last_write_time(cur_path, error);
//         if (error)
//         {
//            std::cerr << error.message() << std::endl;
//            continue;
//         }
//         boost::posix_time::ptime lw_time = boost::posix_time::from_time_t(last_write);
//         files[lw_time] = cur_path;
//      }
//   }
//
// // multimap iterator
//std::map<boost::posix_time::ptime, boost::filesystem::path>::iterator pos;
//
//// print the data
//for(pos = files.begin(); pos != files.end(); ++pos)
//{
//input_files.push_back(pos->second);
//}



  ////processing loop
  int answer;
  bool escape=false;

  for(int i=0;i<input_files.size();i++)
  {
    if(boost::filesystem::is_directory(input_files[i]))continue;
    else 
    {
      bool known_ft=false;
      for(int j=0;j<supported_filetypes.size();j++)
      {
      //if (input_files[i].extension().compare(supported_filetypes[j])==0)known_ft=true;
      if (input_files[i].extension()==supported_filetypes[j])known_ft=true;
      }
      if(known_ft )img_list.push_back(input_files[i].c_str());
    }
  }
return true;
}
