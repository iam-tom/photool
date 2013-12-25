#include<boost/filesystem.hpp>
#include<boost/filesystem/operations.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>
#include<map>
#include<ctime>
#include<photo_sort_viewer.h>
#include"photo_utils.h"
void write_to_dir(boost::filesystem::path dir,std::string filename,cv::Mat& img)
  {
      
      dir/=filename;
      cv::imwrite(dir.c_str(),img);
  }

void fit_img(cv::Mat& img)
   {

     float longside;
     if(img.cols>img.rows)longside=img.cols;
     else longside=img.rows;
     float s =1000/longside;

     cv::resize(img,img,cv::Size(img.cols*s,img.rows*s));

   }

int main(int argc, const char *argv[])
{
  boost::filesystem::path input_dir(argv[1]);
  std::vector<boost::filesystem::path> input_files;
  boost::filesystem::path infofile_path=input_dir/"tags.xml";
  std::vector<std::string>supported_filetypes;
  // known filetypes
  supported_filetypes.push_back(".JPG");
  supported_filetypes.push_back("jpg");
  supported_filetypes.push_back("PNG");
  supported_filetypes.push_back("png");

  // apply filter operation

  bool filter=false;
  int filter_val=-100;
  int  start_val=0;
  if(argc>2)
  {
    filter = true;
    filter_val=atoi(argv[2]);
    std::cout<<"filtering:"<<filter_val<<std::endl;
  }
  if(argc>3)
  {
    start_val=atoi(argv[3]);
    std::cout<<"starting at:"<<start_val<<std::endl;
  }



//    // copy content of directory to input files vector
//    //std::copy(boost::filesystem::directory_iterator(input_dir), boost::filesystem::directory_iterator(), std::back_inserter(input_files));
//     boost::filesystem::path path(input_dir);
//     std::map<boost::posix_time::ptime, boost::filesystem::path> files;
//     for (boost::filesystem::directory_iterator iter(path); iter != boost::filesystem::directory_iterator();
//     ++iter)
//     {
//        const boost::filesystem::path& cur_path = iter->path();
//        if (boost::filesystem::is_regular_file(cur_path))
//        {
//           boost::system::error_code error;
//           std::time_t last_write = boost::filesystem::last_write_time(cur_path, error);
//           if (error)
//           {
//              std::cerr << error.message() << std::endl;
//              continue;
//           }
//           boost::posix_time::ptime lw_time = boost::posix_time::from_time_t(last_write);
//           files[lw_time] = cur_path;
//        }
//     }
//  
//   // multimap iterator
//  std::map<boost::posix_time::ptime, boost::filesystem::path>::iterator pos;
//  
//  // print the data
//  int i=0;
//  for(pos = files.begin(); pos != files.end(); ++pos)
//  {
//  input_files.push_back(pos->second);
//  i++;
//  }
//  
//  
//    //boost::filesystem::path d1=input_dir;
//    //d1/="dir_1";
//    //boost::filesystem::path d2=input_dir;
//    //d2/="dir_2";
//    //boost::filesystem::path d3=input_dir;
//    //d3/="dir_3";
//   // if(!boost::filesystem::is_directory(d1))boost::filesystem::create_directory(d1);
//   // if(!boost::filesystem::is_directory(d2))boost::filesystem::create_directory(d2);
//   // if(!boost::filesystem::is_directory(d3))boost::filesystem::create_directory(d3);
//   //
//   //
//   // FILTER
//    //std::vector<int>rating_list;
//    //if(filter)
//    //{
//    //if(!boost::filesystem::is_regular(infofile_path))
//    //{
//    //  std::cout<<"no filtering possible"<<std::endl;
//    //}
//    //else
//    //{
//    //    cv::FileStorage boost::filesystem(infofile_path.c_str(), cv::FileStorage::READ);
//    //    for(int i=0;i<input_files.size();i++)
//    //    {
//    //    std::string file=input_files[i].stem().c_str();
//    //    //TODO handle when image not in file
//    //    int rating=boost::filesystem[file]["rating"];
//    //    rating_list.push_back(rating);
//    //    }
//    //    boost::filesystem.release();
//    //}
//    //}
//    //else
//    //{
//    //  for(int i=0;i<input_files.size();++i)
//    //  {
//    //    rating_list.push_back(100);
//    //  }
//    //}
//  
//    ////processing loop
//    std::cout<<input_dir<<std::endl;
//    int answer;
//    bool escape=false;
//    std::vector<boost::filesystem::path> img_list;
//  
//    for(int i=0;i<input_files.size();i++)
//    {
//      if(boost::filesystem::is_directory(input_files[i]))continue;
//      else 
//      {
//        bool known_ft=false;
//        for(int j=0;j<supported_filetypes.size();j++)
//        {
//        //if (input_files[i].extension().compare(supported_filetypes[j])==0)known_ft=true;
//        if (input_files[i].extension()==supported_filetypes[j])known_ft=true;
//        }
//        if(known_ft )img_list.push_back(input_files[i]);
//      }
//    }

  std::vector<boost::filesystem::path> img_list;
  get_img_list(input_dir,img_list);
  for(int i=0;i<img_list.size();i++)
  {
    std::cout<<img_list[i].c_str()<<std::endl;;
  }
  
  cv::Size vsize=cv::Size(1366,768);
  //cv::Size vsize=cv::Size(1920,1080);
  //cv::Size vsize=cv::Size(2500,1500);
  Viewer viewer(vsize,Viewer::TRIPLE);

  //boost::filesystem::path testpath="data/test.xml";
  viewer.load_image_list(img_list);
  viewer.load_tag_list(infofile_path);
  viewer.filter(filter_val);
  viewer.setStart(start_val);
  viewer.run();
  std::cout<<"saving list"<<std::endl;
  viewer.save_tag_list(infofile_path);

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
  //std::cout<<input_files[i]<<std::endl;
  //cv::Mat input_img=cv::imread(input_files[i].c_str(),-1);

  //cv::Mat display_img;
  //input_img.copyTo(display_img);
  //fit_img(display_img);

  //answer=0;
  //cv::imshow("CURRENT",display_img);
  //answer=cv::waitKey(0);



  //// swich "callbacks"
  //switch( answer)
  //{


  //  // 1 //
  //  case 1048625:
  //    {
  //    write_to_dir(d1,input_files[i].filename().c_str(),input_img);
  //      break;
  //    }
  //  case 1114033:
  //    {
  //    write_to_dir(d1,input_files[i].filename().c_str(),input_img);
  //      break;
  //    }
  //  // 2 //
  //  case 1048626:
  //    {
  //    write_to_dir(d2,input_files[i].filename().c_str(),input_img);
  //      break;
  //    }
  //  case 1114034:
  //    {
  //    write_to_dir(d2,input_files[i].filename().c_str(),input_img);
  //      break;
  //    }
  //  // 3 //
  //  case 1048627:
  //    {
  //    write_to_dir(d3,input_files[i].filename().c_str(),input_img);
  //      break;
  //    }
  //  case 1114035:
  //    {
  //    write_to_dir(d3,input_files[i].filename().c_str(),input_img);
  //      break;
  //    }
  //    // ESC //////
  //  case 1048603:
  //    {
  //      escape=true;
  //    }
  //}



  //if(escape)break;
  //}
  return 0;
}





