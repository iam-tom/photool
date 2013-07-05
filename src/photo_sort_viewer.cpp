#include<photo_sort_viewer.h>


Viewer::Viewer()
{ std::cout<<"instantiating viever"<<std::endl;




  stop_file_=cv::imread("data/stop.png",-1);
  curr_ctr_=0;
  prev_ctr_=-1;
  next_ctr_=1;

  curr_window_.size=cv::Size(960,1000);
  curr_window_.name="CURRENT IMAGE";
  curr_window_.pos=cv::Point2f(460,0);

  prev_window_.size=cv::Size(620,475);
  prev_window_.name="PREVIOUS IMAGE";
  prev_window_.pos=cv::Point2f(1300,0);

  next_window_.size=cv::Size(620,475);
  next_window_.name="NEXT IMAGE";
  next_window_.pos=cv::Point2f(1300,525);
}


Viewer::Viewer(cv::Size&)
{
  std::cout<<"variable screen size not yet available"<<std::endl;
}

bool Viewer::load_image_list(std::vector<boost::filesystem::path>& img_list)
{
img_list_=img_list;
num_images_=(int)img_list.size();
rating_list_.resize(num_images_,0);
return true;
}


void Viewer::run()
{
  // start at beginning of list
  std::string curr_path=img_list_[curr_ctr_].c_str();
  std::string next_path=img_list_[next_ctr_].c_str();

   curr_file_=cv::imread(curr_path,-1);
   prev_file_=stop_file_;
   next_file_=cv::imread(next_path,-1);

  //
  display(curr_file_,prev_file_,next_file_);
}
void Viewer::overlayRating(cv::Mat& img)
{
  std::string rating ="Rating: "+boost::lexical_cast<std::string>(rating_list_[curr_ctr_]);
  int fontFace = cv::FONT_HERSHEY_PLAIN;
  double fontScale = 3;
  int thickness = 1;
  cv::Point textOrg(0, img.rows-50);
  cv::putText(img, rating, textOrg, fontFace, fontScale, cv::Scalar::all(255), thickness,1);
}
void Viewer::display(cv::Mat curr_file,cv::Mat prev_file,cv::Mat next_file)
{

    fit_img(curr_file,curr_window_.size);
    fit_img(prev_file,prev_window_.size);
    fit_img(next_file,next_window_.size);


    overlayRating(curr_file);

    cv::namedWindow(curr_window_.name);
    cv::moveWindow(curr_window_.name,curr_window_.pos.x,curr_window_.pos.y);
    //cv::resizeWindow(curr_window_.name,curr_window_.size.width,curr_window_.size.height);
    cv::imshow(curr_window_.name,curr_file);

    cv::namedWindow(prev_window_.name);
    cv::moveWindow(prev_window_.name,curr_window_.pos.x-(prev_file.cols+10),0);
    //cv::resizeWindow(prev_window_.name,prev_window_.size.width,prev_window_.size.height);
    cv::imshow(prev_window_.name,prev_file);

    cv::namedWindow(next_window_.name);
    cv::moveWindow(next_window_.name,curr_window_.pos.x+(curr_file.cols+10),0);
    //cv::resizeWindow(next_window_.name,next_window_.size.width,next_window_.size.height);
    cv::imshow(next_window_.name,next_file);

    int key;
    key=cv::waitKey(0);
    //std::cout<<key<<std::endl;
    parseKey(key);


    //cv::namedWindow(next_window_.name);
    //cv::namedWindow(prev_window_.name);






}

void Viewer::rate(int rating)
{
  rating_list_[curr_ctr_]=rating;
  //std::cout<<"RATING "<<rating_list_[curr_ctr_]<<std::endl;
  display(curr_file_,prev_file_,next_file_);
}
void Viewer::navigate(NAV_CMD cmd)
{
    switch(cmd)
    {
      case NEXT:
        {
          curr_ctr_++;
          prev_ctr_++;
          next_ctr_++;

          prev_file_=curr_file_;
          curr_file_=next_file_;
          if(next_ctr_>num_images_)
          {
            next_file_=stop_file_;
          }
          else
          {
            std::string next_path=img_list_[next_ctr_].c_str();
            next_file_=cv::imread(next_path,-1);
          }
            break;
        }
      case PREV:
        {
          curr_ctr_--;
          prev_ctr_--;
          next_ctr_--;

          next_file_=curr_file_;
          curr_file_=prev_file_;
          if(prev_ctr_<0)
          {
            prev_file_=stop_file_;
          }
          else
          {
            std::string prev_path=img_list_[prev_ctr_].c_str();
            prev_file_=cv::imread(prev_path,-1);
          }
            break;
        }
    }
    display(curr_file_,prev_file_,next_file_);
}

void Viewer::loadInfofile(boost::filesystem::path& file)
{


//TODO make possible to load rating and other information to images in folder via infofile
//TODO set cwd automatically and in combination with tags in info file avoid load filelistrating
//TODO every file in file_list is searched for in xml - info loaded or default values set
cv::FileStorage fs(file.c_str(), cv::FileStorage::READ);

// first method: use (type) operator on FileNode.

for(int i=0;i<num_images_;i++)
{
std::string file=img_list_[i].stem().c_str();
//TODO handle when image not in file
int rating=fs[file]["rating"];
rating_list_[i]=rating;
}
fs.release();

}


void Viewer::saveInfofile()
{

  //TODO use cv Filestorage 
  //TODOGENERIC PATH
  //list of images
cv::FileStorage fs;
fs.open("data/test.xml", cv::FileStorage::WRITE);
// Check if we actually created the file
if(fs.isOpened())
{


    for(int i = 0; i < (int)img_list_.size(); i++)
    {
        fs << img_list_[i].stem().c_str()<<"{";
        fs <<"rating"<<rating_list_[i];
        fs <<"id_no"<<9999;
        fs <<"id_name"<<"Unknown";
        fs<<"}";
    }

    fs.release();
}

}
void Viewer::prepareExit()
{
  //TODO: dump xml file with ratings and so on..
  saveInfofile();
  exit(1);
}
void Viewer::parseKey(int& key)
{

  // swich "callbacks"
  switch( key)
  {
    bool escape;

    case 1113939:
      {
        navigate(Viewer::NEXT);
        break;
      }
    case 1113937:
      {
        navigate(Viewer::PREV);
        break;
      }


    case 1048625:
      {
        rate(1);
        break;
      }
    case 1114033:
      {
        rate(1);
        break;
      }
    // 2 //
    case 1048626:
      {
        rate(2);
        break;
      }
    case 1114034:
      {
        rate(2);
        break;
      }
    // 3 //
    case 1048627:
      {
        rate(3);
        break;
      }
    case 1114035:
      {
        rate(3);
        break;
      }
      // ESC //////
    case 1048603:
      {
        prepareExit();
      }
  }
}
void Viewer::fit_img(cv::Mat& img,cv::Size& win_size)
   {

     float longside,limit;
     if(img.cols>img.rows)
     {
       longside=img.cols;
       limit=(float)win_size.height;
     }
     else 
     {
       longside=img.rows;
       limit=(float)win_size.height;
     }
     float s =(float)limit/longside;

     cv::resize(img,img,cv::Size(round(img.cols*s),round(img.rows*s)));

}
