#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<opencv2/xfeatures2d/nonfree.hpp>
#include<opencv2/xfeatures2d.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/calib3d.hpp>
#include<iostream>
using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;
void pipei1(Mat image_temp, Mat image_roi);
void pipei2(Mat image_temp, Mat image_src);
void pipei3(Mat image_temp, Mat image_src);
Mat scale_back_white(Mat image_temp,Mat image_roi);
Mat scale_trans_gray(Mat image_temp, Mat image_src);
Mat scale_trans_gray(Mat image_temp);
void temp_match(Mat src_image,Mat temp_image,Mat temp_image_out,int MODE);