#include<photo_sort_viewer.h>


Viewer::Viewer()
{ 
  std::cout<<"instantiating viever"<<std::endl;
  debug_=false;
  crash_recovery_=false;
  crash_file_path_=NULL;



  stop_file_=cv::imread("/home/tom/workspace/cpp/photool/data/stop.png",-1);
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


Viewer::Viewer(cv::Size& vsize,Viewer::MODE mode)
{
  //std::cout<<"variable screen size not yet available"<<std::endl;
  crash_recovery_=false;
  crash_file_path_=NULL;
  stop_file_=cv::imread("/home/tom/workspace/cpp/photool/data/stop.png",-1);
  curr_ctr_=0;
  prev_ctr_=-1;
  next_ctr_=1;


  int WINDOW_OFFSET=80;
  switch (mode)
  {
    case TRIPLE:
      {
        curr_window_.size=cv::Size(round(vsize.width/2),vsize.height-WINDOW_OFFSET);
        curr_window_.name="CURRENT IMAGE";
        curr_window_.pos=cv::Point2f(round(vsize.width/4),0);
        curr_window_.display_window=true;

        prev_window_.size=cv::Size(round(vsize.width/4),(round(vsize.height/2))-WINDOW_OFFSET);
        prev_window_.name="PREVIOUS IMAGE";
        prev_window_.pos=cv::Point2f(1300,0);
        prev_window_.display_window=true;

        next_window_.size=cv::Size(round(vsize.width/4),(round(vsize.height/2))-80);
        next_window_.name="NEXT IMAGE";
        next_window_.pos=cv::Point2f(1300,200);
        next_window_.display_window=true;
        break;
      }
    case FULLSCREEN:
      {
        curr_window_.size=cv::Size(vsize.width,vsize.height-WINDOW_OFFSET);
        curr_window_.name="CURRENT IMAGE";
        curr_window_.pos=cv::Point2f(0,0);
        curr_window_.display_window=true;
        // prev_window_.size=cv::Size(round(vsize.width/4),round(vsize.height/2));
        // prev_window_.name="PREVIOUS IMAGE";
        // prev_window_.pos=cv::Point2f(1300,0);
        prev_window_.display_window=false;

        //next_window_.size=cv::Size(round(vsize.width/4),round(vsize.height/2));
        //next_window_.name="NEXT IMAGE";
        //next_window_.pos=cv::Point2f(1300,525);
        next_window_.display_window=false;
        break;
      }

    default:
      {
        std::cout<<"viewer Mode not supported"<<std::endl;
        break;
      }
  }


}

bool Viewer::load_image_list(std::vector<boost::filesystem::path>& img_list)
{
img_list_=img_list;
num_images_=(int)img_list.size();
rating_list_.resize(num_images_,0);
rotation_list_.resize(num_images_,0);
filter_list_.resize(num_images_,false);
return true;
}


void Viewer::run()
{
  // start at beginning of list
  std::string curr_path;
  std::string next_path;
  std::string prev_path;

   if(prev_ctr_>=0 && next_ctr_<img_list_.size())
   {
    curr_path=img_list_[curr_ctr_].c_str();
    next_path=img_list_[next_ctr_].c_str();
    prev_path=img_list_[prev_ctr_].c_str();

    curr_file_=cv::imread(curr_path,-1);
    prev_file_=cv::imread(prev_path,-1);
    next_file_=cv::imread(next_path,-1);
   }
  else if(prev_ctr_<0 && next_ctr_<img_list_.size())
  {
   curr_path=img_list_[curr_ctr_].c_str();
   next_path=img_list_[next_ctr_].c_str();

   curr_file_=cv::imread(curr_path,-1);
   next_file_=cv::imread(next_path,-1);
   prev_file_=stop_file_;
  }
  else if(prev_ctr_>=0 && next_ctr_>=img_list_.size())
  {
   curr_path=img_list_[curr_ctr_].c_str();
   prev_path=img_list_[prev_ctr_].c_str();
   curr_file_=cv::imread(curr_path,-1);
   prev_file_=cv::imread(prev_path,-1);
   next_file_=stop_file_;
  }

  //
  display(curr_file_,prev_file_,next_file_);
}
void Viewer::overlayRating(cv::Mat& img)
{
  std::string rating;
  if(rating_list_[curr_ctr_]<0) 
  {
   rating ="Rating: - ";
  }
  else
  {
  rating ="Rating: "+boost::lexical_cast<std::string>(rating_list_[curr_ctr_]);
  }
  int fontFace = cv::FONT_HERSHEY_PLAIN;
  double fontScale = 3;
  int thickness = 2;
  cv::Point textOrg(0, img.rows-50);
  cv::putText(img, rating, textOrg, fontFace, fontScale, cv::Scalar::all(255), thickness,1);
}
void Viewer::applyRotation(cv::Mat& img,int& ctr)
{
  switch( rotation_list_[ctr])
  {
    case 0:
      {
        //std::cout<<"no rotation"<<std::endl;
        break;
      }
    case 1:
      {
        //std::cout<<"cw 90"<<std::endl;
        cv::transpose(img,img);
        cv::flip(img,img,1);
        break;
      }
    case -1:
      {
        //std::cout<<"ccw 90"<<std::endl;
        cv::transpose(img,img);
        cv::flip(img,img,0);
        break;
      }
    default:
      {
        std::cout<<"rotation value not defined"<<std::endl;
      }


      return;

  }
}

void Viewer::display(cv::Mat& curr_file,cv::Mat& prev_file,cv::Mat& next_file)
{

  //------------------make diplay operations for current window
      cv::Mat n_f;
      cv::Mat c_f;
      cv::Mat p_f;

    if(curr_window_.display_window)
       {
          curr_file.copyTo(c_f);


          fit_img(c_f,curr_window_.size);
          applyRotation(c_f,curr_ctr_);
          overlayRating(c_f);
          cv::namedWindow(curr_window_.name);
          cv::moveWindow(curr_window_.name,curr_window_.pos.x,curr_window_.pos.y);
          //cv::resizeWindow(curr_window_.name,curr_window_.size.width,curr_window_.size.height);
          cv::imshow(curr_window_.name,c_f);
       }

    if(prev_window_.display_window)
    {
        prev_file.copyTo(p_f);

      //------------------make diplay operations for previous window
        fit_img(p_f,prev_window_.size);
        applyRotation(p_f,prev_ctr_);
        cv::namedWindow(prev_window_.name);
        cv::moveWindow(prev_window_.name,curr_window_.pos.x-(p_f.cols+10),0);
        //cv::resizeWindow(prev_window_.name,prev_window_.size.width,prev_window_.size.height);
        cv::imshow(prev_window_.name,p_f);
    }


    if(next_window_.display_window)
    {
      next_file.copyTo(n_f);
      //------------------make diplay operations for next window
 
      fit_img(n_f,next_window_.size);
      applyRotation(n_f,next_ctr_);
      cv::namedWindow(next_window_.name);
      cv::moveWindow(next_window_.name,curr_window_.pos.x+(c_f.cols+10),0);
      if(debug_)std::cout<<"position next window:"<<curr_window_.pos.x+(c_f.cols+10)<<std::endl;
      //cv::resizeWindow(next_window_.name,next_window_.size.width,next_window_.size.height);
      cv::imshow(next_window_.name,n_f);

    }


    //  parse keys

    int key;
    key=cv::waitKey(0);
    parseKey(key);


}

void Viewer::rotate(Viewer::CMD cmd)
{
  switch(cmd)
  {
    case ROT_CW:
      {
        if(rotation_list_[curr_ctr_]!=1)rotation_list_[curr_ctr_]+=1;
        break;
      }
    case ROT_CCW:
      {
        if(rotation_list_[curr_ctr_]!=-1)rotation_list_[curr_ctr_]-=1;
        break;
      }
    default:
      {
        std::cout<<"invalid command"<<std::endl;
        break;
      }
  }
    display(curr_file_,prev_file_,next_file_);
}
void Viewer::rate(int rating)
{
  rating_list_[curr_ctr_]=rating;
  //std::cout<<"RATING "<<rating_list_[curr_ctr_]<<std::endl;
  display(curr_file_,prev_file_,next_file_);
}
void Viewer::navigate(CMD cmd)
{
    switch(cmd)
    {
      case NEXT:
        {

          // if stop image is already reached
          if(next_ctr_==num_images_)
          {
          }
          else if(next_ctr_==num_images_-1)
          {
            prev_file_=curr_file_;
            curr_file_=next_file_;
            next_file_=stop_file_;

            prev_ctr_=curr_ctr_;
            curr_ctr_=next_ctr_;
            next_ctr_++;

          }
          else
          {
            prev_ctr_=curr_ctr_;
            curr_ctr_=next_ctr_;
            next_ctr_++;

            // filter images
            std::string next_path;
            while(next_ctr_<num_images_)
            {
              if(filter_list_[next_ctr_]==false)
              {
                next_path=img_list_[next_ctr_].c_str();
                prev_file_=curr_file_;
                curr_file_=next_file_;
                next_file_=cv::imread(next_path,-1);
                break;
              }
              else next_ctr_++;
              if(next_ctr_==num_images_-1)
              {
                prev_file_=curr_file_;
                curr_file_=next_file_;
                next_file_=stop_file_;

                prev_ctr_=curr_ctr_;
                curr_ctr_=next_ctr_;
                next_ctr_++;
                break;
              }
            }
          }
            break;
        }
      case PREV:
        {

          if(prev_ctr_<0)
          {
          }
          else if(prev_ctr_==0)
          {
            next_file_=curr_file_;
            curr_file_=prev_file_;
            prev_file_=stop_file_;

            next_ctr_=curr_ctr_;
            curr_ctr_=prev_ctr_;
            prev_ctr_--;
          }
          else
          {
            next_ctr_=curr_ctr_;
            curr_ctr_=prev_ctr_;
            prev_ctr_--;

            // filter images
            std::string prev_path;
            while(prev_ctr_>=0)
            {
              if(filter_list_[prev_ctr_]==false)
              {
                prev_path=img_list_[prev_ctr_].c_str();
                next_file_=curr_file_;
                curr_file_=prev_file_;
                prev_file_=cv::imread(prev_path,-1);
                break;
              }
              else prev_ctr_--;
              if(prev_ctr_==0)
              {
                next_file_=curr_file_;
                curr_file_=prev_file_;
                prev_file_=stop_file_;

                next_ctr_=curr_ctr_;
                curr_ctr_=prev_ctr_;
                prev_ctr_--;
                break;
              }
            }
          }
            break;
        }
    }
    // display updated files
    if(debug_)std::cout<<"PREV CTR="<<prev_ctr_<<std::endl;
    if(debug_)std::cout<<"CURR CTR="<<curr_ctr_<<std::endl;
    if(debug_)std::cout<<"NEXT CTR="<<next_ctr_<<std::endl;
    if(debug_)std::cout<<"-----------------"<<std::endl;

    //save counter to crash file to avoid haveing to start all over again in
    //case of crash
    if(crash_recovery_)this->save_crash_file();
    this->display(curr_file_,prev_file_,next_file_);
}

void Viewer::activate_crash_recovery(boost::filesystem::path& crash_file_path)
{
  
  crash_file_path_=new boost::filesystem::path(crash_file_path);
  crash_recovery_=true;
}
void Viewer::save_crash_file()
{

cv::FileStorage fs;
fs.open(crash_file_path_->c_str(), cv::FileStorage::WRITE);
if(fs.isOpened())
{
  fs<<"counter"<<curr_ctr_;

  fs.release();
}

}
void Viewer::load_crash_file(int & counter)
{

  if(crash_recovery_)
  {
    if(boost::filesystem::is_regular(*crash_file_path_))
    {
      std::cout<<"found crash file - start where left off(y/n)?"<<std::endl;
      char answer;
      std::cin>>answer;
      if(answer=='y')
      {
      cv::FileStorage fs(crash_file_path_->c_str(), cv::FileStorage::READ);

      counter=(int)fs["counter"];
      }
    }
  }
}
void Viewer::load_tag_list(boost::filesystem::path& file)
{

  if(!boost::filesystem::is_regular(file))
  {
    std::cout<<"no tag file loading default ratings"<<std::endl;
    rotation_list_.resize(num_images_);
    rating_list_.clear();
    rating_list_.resize(num_images_);
    rating_list_.clear();
    for(int i=0;i<num_images_;++i)
       {
         rating_list_[i]=-1;
         rotation_list_[i]=0;
       }
    return;
  }

//TODO make possible to load rating and other information to images in folder via infofile
//TODO set cwd automatically and in combination with tags in info file avoid load filelistrating
//TODO every file in file_list is searched for in xml - info loaded or default values set
cv::FileStorage fs(file.c_str(), cv::FileStorage::READ);


for(int i=0;i<num_images_;i++)
{
std::string file=img_list_[i].stem().c_str();
//TODO handle when image not in file
int rating=fs[file]["rating"];
rating_list_[i]=rating;
int rotation=fs[file]["rotation"];
rotation_list_[i]=rotation;
}
fs.release();

}

void Viewer::filter(int val)
{
  for(int i=0;i<num_images_;++i)
  {
    if (rating_list_[i]<val)filter_list_[i]=true;
    std::cout<<filter_list_[i]<<std::endl;
  }
  //set counter to appropriate values
  prev_ctr_=-1;
  bool curr_ctr_set, next_ctr_set;
  curr_ctr_set=false;
  next_ctr_set=false;
  for(int i=0;i<num_images_;i++)
  {
    if(filter_list_[i]==false)
    {
      curr_ctr_=i;
      curr_ctr_set=true;
      break;
    }
  }
  if(curr_ctr_set)
  {
    for(int i=curr_ctr_+1 ;i<num_images_;i++)
    {
      if(filter_list_[i]==false)
      {
        next_ctr_=i;
        next_ctr_set=true;
        break;
      }
    }
  }

  std::cout<<"----------------"<<std::endl;
  std::cout<<curr_ctr_<<std::endl;
  std::cout<<next_ctr_<<std::endl;
  if(!curr_ctr_set || !next_ctr_set)
  {
    std::cout<<"FILTERING yields invalid file list - exiting"<<std::endl;
    exit(1);
  }

}

void Viewer::save_tag_list(boost::filesystem::path& file)
{

  //TODO use cv Filestorage 
  //TODOGENERIC PATH
  //list of images
cv::FileStorage fs;
fs.open(file.c_str(), cv::FileStorage::WRITE);
// Check if we actually created the file
if(fs.isOpened())
{


    for(int i = 0; i < (int)img_list_.size(); i++)
    {
        fs << img_list_[i].stem().c_str()<<"{";
        fs <<"rating"<<rating_list_[i];
        fs <<"rotation"<<rotation_list_[i];
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
  //saveInfofile();
  //exit(1);
}
void Viewer::parseKey(int& key)
{
  //std::cout<<key<<std::endl;

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

    case 1113938:
      {

        rotate(Viewer::ROT_CW);
        break;
      }
    case 1113940:
      {
        rotate(Viewer::ROT_CCW);
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
        std::cout<<"EXIT"<<std::endl;
        //prepareExit();
      }
    default:
      {
        std::cout<<"KEY NOT DEFINED"<<std::endl;
        break;
      }

  }
}
void Viewer::fit_img(cv::Mat& img,cv::Size& win_size)
   {
     float longside,limit;
     if(img.cols>img.rows)
     {
       longside=img.rows;
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
void Viewer::setStart(int no)
{
if(crash_recovery_)
{
this->load_crash_file(no);
}
std::cout<<"loaded "<< no<<std::endl;
curr_ctr_=no;
prev_ctr_=no-1;
next_ctr_=no+1;

}
