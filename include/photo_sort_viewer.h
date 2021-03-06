
#include<iostream>


#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<boost/filesystem.hpp>
#include<boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

class Viewer
{
  public:
    enum CMD
    {
      NEXT,
      PREV,
      ROT_CW,
      ROT_CCW,
    };
    enum MODE
    {
      FULLSCREEN,
      TRIPLE,
    };
    struct window{
      std::string name;
      cv::Size size;
      cv::Point2f pos;
      bool display_window;
    };

    Viewer();
    Viewer(cv::Size& screen_size,Viewer::MODE mode);
    virtual ~Viewer()
    {
      if(crash_file_path_)delete crash_file_path_;
    };

    void run();
    bool load_image_list(std::vector<boost::filesystem::path>& img_list);
    void saveInfofile();
    void load_tag_list(boost::filesystem::path& file);
    void save_tag_list(boost::filesystem::path& file);
    void filter(int val);
    void setStart(int no);


    void activate_crash_recovery(boost::filesystem::path & crash_file_path);

    protected:
    void save_crash_file();
    void load_crash_file(int& counter);
    boost::filesystem::path* crash_file_path_;
    boost::posix_time::time_facet* viewer_time_;
    bool crash_recovery_;

    void fit_img(cv::Mat& img,cv::Size& win_size);
    void display(cv::Mat& curr_img,cv::Mat& prev_img,cv::Mat& next_img);
    void parseKey(int& key);
    void navigate(CMD cmd);
    void rate(int rating);
    void overlayRating(cv::Mat& img);
    void applyRotation(cv::Mat& img,int& ctr);
    void rotate(Viewer::CMD cmd);
    void prepareExit();
    window curr_window_;
    window prev_window_;
    window next_window_;
    std::vector<boost::filesystem::path> img_list_;
    boost::filesystem::path tag_list_;
    cv::Mat curr_file_;
    cv::Mat prev_file_;
    cv::Mat next_file_;
    int curr_ctr_;
    int prev_ctr_;
    int next_ctr_;

    cv::Mat stop_file_;
    int num_images_;
    bool debug_;



    std::vector<int>rating_list_;
    std::vector<int>rotation_list_;
    std::vector<bool>filter_list_;
};
