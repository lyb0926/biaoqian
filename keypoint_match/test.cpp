//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include<opencv2/xfeatures2d/nonfree.hpp>
//#include<opencv2/xfeatures2d.hpp>
//#include<opencv2/imgproc.hpp>
//#include<iostream>
//
//using namespace std;
//using namespace cv;
//using namespace cv::xfeatures2d;
//int main()
//{
//	Mat image_temp = imread("C:/Users/test1/Desktop/Ä£°æ/2.bmp");
//	Mat warp_dst; 
//	Point2f srcTri[3];
//	Point2f dstTri[3];
//	srcTri[0] = Point2f(0, 0);
//	srcTri[1] = Point2f(image_temp.cols - 1, 0);
//	srcTri[2] = Point2f(0, image_temp.rows - 1);
//
//	dstTri[0] = Point2f(image_temp.cols * 0.0, image_temp.rows * 0.33);
//	dstTri[1] = Point2f(image_temp.cols * 0.85, image_temp.rows * 0.25);
//	dstTri[2] = Point2f(image_temp.cols * 0.15, image_temp.rows * 0.7);
//	Mat warp_mat = getAffineTransform(srcTri, dstTri);
//	cout << warp_mat;
//	warpAffine(image_temp, warp_dst, warp_mat, image_temp.size(), 1, 0);
//	namedWindow("add", WINDOW_NORMAL);
//	imshow("add", warp_dst);
//	waitKey(0);
//}
//
