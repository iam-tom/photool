#include<boost/filesystem.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<photo_sort_viewer.h>
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


  // copy content of directory to input files vector
  std::copy(boost::filesystem::directory_iterator(input_dir), boost::filesystem::directory_iterator(), std::back_inserter(input_files));


  //create dirs
  boost::filesystem::path d1=input_dir;
  d1/="dir_1";
  boost::filesystem::path d2=input_dir;
  d2/="dir_2";
  boost::filesystem::path d3=input_dir;
  d3/="dir_3";
 // if(!boost::filesystem::is_directory(d1))boost::filesystem::create_directory(d1);
 // if(!boost::filesystem::is_directory(d2))boost::filesystem::create_directory(d2);
 // if(!boost::filesystem::is_directory(d3))boost::filesystem::create_directory(d3);

  ////processing loop
  std::cout<<input_dir<<std::endl;
  int answer;
  bool escape=false;
  std::vector<boost::filesystem::path> img_list;

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
      std::cout<<known_ft<<std::endl;
      if(known_ft)img_list.push_back(input_files[i]);
    }
  }
  cv::Size vsize=cv::Size(1366,768);
  Viewer viewer(vsize);

  //boost::filesystem::path testpath="data/test.xml";
  viewer.load_image_list(img_list);
  viewer.load_tag_list(infofile_path);
  viewer.run();
  std::cout<<"intending to save list"<<std::endl;
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





