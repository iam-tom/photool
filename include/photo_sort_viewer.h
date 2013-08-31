
#include<iostream>


#include<opencv/cv.h>
#include<opencv/highgui.h>

#include<boost/filesystem.hpp>
#include<boost/lexical_cast.hpp>

class Viewer
{
  public:
    enum NAV_CMD
    {
      NEXT,
      PREV
    };
    struct window{
      std::string name;
      cv::Size size;
      cv::Point2f pos;
    };

    Viewer();
    Viewer(cv::Size& screen_size);
    virtual ~Viewer(){};

    void run();
    bool load_image_list(std::vector<boost::filesystem::path>& img_list);
    void saveInfofile();
    void load_tag_list(boost::filesystem::path& file);
    void save_tag_list(boost::filesystem::path& file);

    protected:
    void fit_img(cv::Mat& img,cv::Size& win_size);
    void display(cv::Mat curr_img,cv::Mat prev_img,cv::Mat next_img);
    void parseKey(int& key);
    void navigate(NAV_CMD cmd);
    void rate(int rating);
    void overlayRating(cv::Mat& img);
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



    std::vector<int>rating_list_;
};
