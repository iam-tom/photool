#include"photo_importer.h"

photo_importer::~photo_importer()
{
  //if(progress_bar_) delete progress_bar_;
}
bool photo_importer::make_folders(boost::filesystem::path& base_folder)
{

   raw_folder_=base_folder/"raw";
   jpg_folder_=base_folder/"jpg";
   snapshot_folder_=base_folder/"snapshot";


  if(boost::filesystem::is_directory(base_folder))
      {
        if(!boost::filesystem::is_directory(raw_folder_))boost::filesystem::create_directory(raw_folder_);
        if(!boost::filesystem::is_directory(jpg_folder_))boost::filesystem::create_directory(jpg_folder_);
        if(!boost::filesystem::is_directory(snapshot_folder_))boost::filesystem::create_directory(snapshot_folder_);

      }


}
photo_importer::photo_importer(boost::filesystem::path& import_dir,boost::filesystem::path& target_dir)
{
  std::cout<<"********* Photo Importer *********"<<std::endl;
  std::cout<<"importing from :  "<<import_dir<<std::endl;
  std::cout<<"importing to   :  "<<target_dir<<std::endl;


  get_img_list(import_dir,img_list_);
  this->make_folders(target_dir);




}

bool photo_importer::process()
{
  if(img_list_.size()==0) std::cout<<"noting to be done"<<std::endl;
  //init
  vis_=false;
  progress_bar_ = NULL;
  std::cout<<"Processing:"<<std::endl;
  if(vis_&& img_list_.size()>0)progress_bar_ = new boost::progress_display(img_list_.size());

  //process in parallel
#pragma omp parallel for
  for(int i=0;i<img_list_.size();++i)
  {
    std::string ext=img_list_[i].extension().c_str();
    if(ext==".JPG" || ext==".jpg") handle_jpg(img_list_[i]);
    else if(ext==".CR2" || ext=="raw") handle_raw(img_list_[i]);

    if(vis_)
    {
    //progress
    ++(*progress_bar_);
    boost::progress_display show_progress();
    }
  }

}


bool photo_importer::handle_jpg(boost::filesystem::path& file)
{
  cv::Mat img_full;
  //std::cout<<"loading : "<<file.c_str()<<std::endl;
  if(boost::filesystem::exists(file))img_full=cv::imread(file.c_str());
  // landscape
  cv::Size snapshot_size;
  if(img_full.rows<img_full.cols) snapshot_size=cv::Size(1000,round((1000.0/img_full.cols)*img_full.rows));

  if(img_full.rows>img_full.cols) snapshot_size=cv::Size(round(1000/img_full.rows*img_full.cols),1000);

  cv::Mat img_snapshot(snapshot_size,img_full.type());
  cv::resize(img_full,img_snapshot,snapshot_size);

  //dump files
  boost::filesystem::path f_jpg=jpg_folder_/file.filename();
  boost::filesystem::path f_snapshot=snapshot_folder_/file.filename();
  boost::filesystem::copy_file(file,f_jpg);
  cv::imwrite(f_snapshot.c_str(),img_snapshot);

  if(vis_)
  {
    cv::imshow("snapshot",img_snapshot);
    cv::waitKey(20);
  }
}
bool photo_importer::handle_raw(boost::filesystem::path& file)
{
  //std::cout<<"loading : "<<file.c_str()<<std::endl;
  //dump files
  boost::filesystem::path f_raw=raw_folder_/file.filename();
  boost::filesystem::copy_file(file,f_raw);

}


int main (int argc,const char* argv[])
{

  boost::filesystem::path import_dir(argv[1]);
  boost::filesystem::path target_dir(argv[2]);
  photo_importer pi(import_dir,target_dir);
  pi.process();


}
