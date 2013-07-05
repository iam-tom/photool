#include<iostream>
#include<opencv/cv.h>
#include<opencv/highgui.h>




void load_img(std::string& input_string,cv::Mat& img)
{
  img=cv::imread(input_string,CV_LOAD_IMAGE_COLOR);
}


void resize_img(cv::Mat& img,float target_dimension)
{
  cv::Size input_size=cv::Size(img.cols,img.rows);

  bool portrait;
  float scale;
  if(img.rows>img.cols)portrait=true;

  if(portrait) scale=target_dimension/input_size.height;
  else scale=target_dimension/input_size.width;

  cv::Size target_size=cv::Size(round(input_size.width* scale),round(input_size.height*scale));

  cv::resize(img,img,target_size,cv::INTER_AREA);
}

void load_watermark(std::string& path,cv::Mat& watermark)
{
  watermark=cv::imread(path,-1);
}

void insert_watermark(cv::Mat& img, cv::Mat& watermark,cv::Mat& output)
{
  int offset=0;
  cv::Mat overlay= cv::Mat::zeros(img.rows,img.cols,img.type());
  cv::Mat a0=cv::Mat::zeros(img.rows,img.cols,CV_8U);
  std::vector<cv::Mat> channels;
  cv::split(img,channels);
  channels.push_back(a0);
  cv::merge(channels,output);
  output.convertTo(output,CV_8UC4);
  cv::Mat dummy=cv::Mat::zeros(img.rows,img.cols,CV_8UC4);
  watermark.copyTo(dummy(cv::Rect(img.cols-(watermark.cols+offset),img.rows-(watermark.rows+offset),watermark.cols,watermark.rows)));
  //output+=dummy;
  cv::addWeighted(output,1,dummy,0.8,0.0,output);
}


int main(int argc, const char *argv[])
{
  int target_dimension=atoi(argv[3]);
  cv::Mat input_img;
  std::string input_string=argv[1];
  load_img(input_string,input_img);
  resize_img(input_img,target_dimension);

  cv::Mat output=cv::Mat(input_img.rows,input_img.cols,CV_8UC4);
  //output=input_img;
  std::string watermark_path="/home/tom/workspace_cpp/photo/data/watermark_circle.png";
  cv::Mat watermark;
  load_watermark(watermark_path,watermark);
  resize_img(watermark,(int)round(target_dimension/8));
  insert_watermark(input_img,watermark,output);
  //

  cv::imwrite(argv[2],output);
  return 0;
}
