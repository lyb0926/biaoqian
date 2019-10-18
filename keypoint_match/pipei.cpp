#include"pipei.hpp"
int main()
{
	Mat image2 = imread("D:/¾ÆÆ¿¼ì²â/ÏßÕó¾ÆÆ¿/ÏßÕó¾ÆÆ¿_0929/Image3.jpg");
	Mat image_temp2 = imread("C:/Users/test1/Desktop/Ä£°æ/gray_word.jpg");
	
	//Mat temp_trans = scale_back_white(image_temp2, image_temp2);
	//Mat roi_trans = scale_back_white(image_temp2, image2);
	//namedWindow("roi_trans", WINDOW_NORMAL);
	//namedWindow("temp_trans", WINDOW_NORMAL);
	//imshow("roi_trans", roi_trans);
	//imshow("temp_trans", temp_trans);
	//pipei2(temp_trans, roi_trans);

	Mat gray_image_roi_trans = scale_trans_gray(image_temp2, image2);
	//Mat gray_image_temp_trans = scale_trans_gray(image_temp2);
	//namedWindow("roi_trans", WINDOW_NORMAL);
	//namedWindow("temp_trans", WINDOW_NORMAL);
	//imshow("roi_trans", gray_image_roi_trans);
	//imshow("temp_trans", gray_image_temp_trans);
	
	//pipei1(gray_image_temp_trans, gray_image_roi_trans);
	waitKey(0);
}