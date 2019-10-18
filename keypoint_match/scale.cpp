#include"pipei.hpp"

static inline bool ContoursSortFun(vector<cv::Point> contour1, vector<cv::Point> contour2)
{
	return (cv::contourArea(contour1) > cv::contourArea(contour2));
}
bool cmpy_left(cv::Point const& a, cv::Point const& b)
{
	return a.x < b.x;
}
bool cmpy_right_x(cv::Point const& a, cv::Point const& b)
{
	return a.x > b.x;
}
bool cmpy_right_y(cv::Point const& a, cv::Point const& b)
{
	return a.y > b.y;
}
Mat scale_back_white(Mat image_temp, Mat image_src)
//int main()
{
	//Mat image_src = imread("D:/¾ÆÆ¿¼ì²â/ÏßÕó¾ÆÆ¿/ÏßÕó¾ÆÆ¿_0929/Image2.jpg");
	//Mat image_temp = imread("C:/Users/test1/Desktop/Ä£°æ/2.jpg");
	Mat image_temp_yuan_word = imread("C:/Users/test1/Desktop/Ä£°æ/7.png");
	Mat image_temp_chan_word = imread("C:/Users/test1/Desktop/Ä£°æ/8.png");
	Mat image_temp_kuo_word = imread("C:/Users/test1/Desktop/Ä£°æ/9.png");
	//cout << image_temp.type() << image_temp_yuan_word.type();
	Mat src_result;
	//Ä£°æÆ¥Åä
	cv::matchTemplate(image_src, image_temp, src_result, 0);
	double minVal, maxVal;
	Point minLoc, maxLoc, matchLoc;
	minMaxLoc(src_result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	matchLoc = minLoc;
	//rectangle(image1, cv::Rect(matchLoc, cv::Size(image_temp.cols, image_temp.rows)), Scalar(0, 0, 255), 2, 8, 0);
	//½ØÈ¡ÓëÄ£°æÆ¥Åä³É¹¦µÄROI
	Rect rect(matchLoc, cv::Size(image_temp.cols, image_temp.rows));
	Mat image_roi = image_src(rect);
	
	Mat image_roi_yuan;
	Rect r_mask_yuan( 0,0, image_roi.size().width - image_roi.size().width / 1.2, image_roi.size().height - image_roi.size().height / 1.5);
	//Rect r_mask(100, 100, 200,200);
	Mat mask_yuan = Mat::zeros(image_roi.size(), image_roi.type());
	mask_yuan(r_mask_yuan).setTo(255);
	image_roi.copyTo(image_roi_yuan, mask_yuan);

	imshow("1", image_roi_yuan);

	Mat yuan_word_result;
	cv::matchTemplate(image_roi_yuan, image_temp_yuan_word, yuan_word_result, TM_SQDIFF);
	double yuan_minVal, yuan_maxVal;
	Point yuan_minLoc, yuan_maxLoc, yuan_matchLoc;
	minMaxLoc(yuan_word_result, &yuan_minVal, &yuan_maxVal, &yuan_minLoc, &yuan_maxLoc, Mat());
	yuan_matchLoc = yuan_minLoc;
	rectangle(image_roi, cv::Rect(matchLoc, cv::Size(image_temp_yuan_word.cols, image_temp_yuan_word.rows)), Scalar(0, 0, 255), 2, 8, 0);
	circle(image_roi, Point(matchLoc.x, matchLoc.y), 8, Scalar(100, 100, 100), 3);
	
	Rect rect_yuan(yuan_matchLoc, cv::Size(image_temp_yuan_word.cols, image_temp_yuan_word.rows));
	Mat image_yuan_roi = image_roi(rect_yuan);
	Mat image_yuan_bin, image_yuan_gray;
	cvtColor(image_yuan_roi, image_yuan_gray, COLOR_BGR2GRAY);
	threshold(image_yuan_gray, image_yuan_bin, 100, 255, THRESH_OTSU);
	vector<vector<Point> >contours;
	vector<Vec4i> hierarchy;
	//²éÕÒÂÖÀª
	int largest_area = 0;
	int largest_contour_index = 0;
	Scalar color(rand() & 255, rand() & 255, rand() & 255);
	findContours(image_yuan_bin, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	for (int i = 0; i < contours.size(); i++)
	{
		double a = contourArea(contours[i], false); // Find the area of contour 
		if (a > largest_area)
		{
			largest_area = a;
			largest_contour_index = i;    //Store the index of largest contour 
			
			
			//drawContours(image_roi, contours, i, color, 1, 8, hierarchy);
			//drawContours(image_yuan_roi, contours, i, color, 1, 8, hierarchy);
			//Rect r = boundingRect(contours[i]);
			//rectangle(image_roi, Point(r.x+ yuan_matchLoc.x, r.y+ yuan_matchLoc.y), Point(r.br().x+yuan_matchLoc.x, r.br().y + yuan_matchLoc.y), color,1);
		}
	}
	Rect r = boundingRect(contours[largest_contour_index]);
	rectangle(image_roi, Point(r.x + yuan_matchLoc.x, r.y + yuan_matchLoc.y), Point(r.br().x + yuan_matchLoc.x, r.br().y + yuan_matchLoc.y), color, 1);
	Point top_left(r.x + yuan_matchLoc.x, r.y + yuan_matchLoc.y);
	
	//×óÏÂ½Ç
	//ÑÚÄ¤

	Rect r_mask(0, image_roi.size().height/1.5, image_roi.size().width - image_roi.size().width / 1.2, image_roi.size().height- image_roi.size().height / 1.5);
	//Rect r_mask(100, 100, 200,200);
	Mat mask = Mat::zeros(image_roi.size(), image_roi.type());
	mask(r_mask).setTo(255);
	Mat image_roi1;
	image_roi.copyTo(image_roi1, mask);
	//imshow("",image_roi1);

	Mat chan_word_result;
	cv::matchTemplate(image_roi1, image_temp_chan_word, chan_word_result,0);
	double chan_minVal, chan_maxVal;
	Point chan_minLoc, chan_maxLoc, chan_matchLoc;
	minMaxLoc(chan_word_result, &chan_minVal, &chan_maxVal, &chan_minLoc, &chan_maxLoc, Mat());
	chan_matchLoc = chan_minLoc;
	//rectangle(image_roi1, cv::Rect(matchLoc, cv::Size(image_temp_chan_word.cols, image_temp_chan_word.rows)), Scalar(0, 0, 255), 2, 8, 0);
	circle(image_roi1, Point(matchLoc.x, matchLoc.y), 8, Scalar(100, 100, 100), 3);

	Rect rect_chan(chan_matchLoc, cv::Size(image_temp_chan_word.cols, image_temp_chan_word.rows));
	Mat image_chan_roi = image_roi1(rect_chan);
	Mat image_chan_bin, image_chan_gray;
	cvtColor(image_chan_roi, image_chan_gray, COLOR_BGR2GRAY);
	threshold(image_chan_gray, image_chan_bin, 100, 255, THRESH_OTSU);
	vector<vector<Point> >contours_chan;
	vector<Vec4i> hierarchy_chan;
	//²éÕÒÂÖÀª
	int largest_area_chan = 0;
	int largest_contour_index_chan = 0;
	
	findContours(image_chan_bin, contours_chan, hierarchy_chan, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	for (int i = 0; i < contours_chan.size(); i++)
	{
		double a = contourArea(contours_chan[i], false); // Find the area of contour 
		if (a > largest_area_chan)
		{
			largest_area_chan = a;
			largest_contour_index_chan = i;    //Store the index of largest contour 


			//drawContours(image_roi, contours, i, color, 1, 8, hierarchy);
			//drawContours(image_yuan_roi, contours, i, color, 1, 8, hierarchy);
			//Rect r = boundingRect(contours[i]);
			//rectangle(image_roi, Point(r.x+ yuan_matchLoc.x, r.y+ yuan_matchLoc.y), Point(r.br().x+yuan_matchLoc.x, r.br().y + yuan_matchLoc.y), color,1);
		}
	}
	Rect r_chan = boundingRect(contours_chan[largest_contour_index_chan]);
	rectangle(image_roi, Point(r_chan.x + chan_matchLoc.x, r_chan.y + chan_matchLoc.y), Point(r_chan.br().x + chan_matchLoc.x, r_chan.br().y + chan_matchLoc.y), color, 1);
	Point bottom_left(r_chan.x + chan_matchLoc.x, r_chan.br().y + chan_matchLoc.y);
	//circle(image_roi, Point(r_chan.x + chan_matchLoc.x, r_chan.br().y + chan_matchLoc.y), 8, Scalar(100, 100, 100), 3);

	//ÓÒ±ß

	Rect kuo_mask(image_roi.size().width / 1.1, 0, image_roi.size().width - image_roi.size().width / 1.1, image_roi.size().height - image_roi.size().height / 1.5);
	//Rect r_mask(100, 100, 200,200);
	Mat mask_k = Mat::zeros(image_roi.size(), image_roi.type());
	mask_k(kuo_mask).setTo(255);
	Mat image_roi_kuo;
	image_roi.copyTo(image_roi_kuo, mask_k);
	

	Mat kuo_word_result;
	cv::matchTemplate(image_roi_kuo, image_temp_kuo_word, kuo_word_result,0);
	double kuo_minVal, kuo_maxVal;
	Point kuo_minLoc, kuo_maxLoc, kuo_matchLoc;
	minMaxLoc(kuo_word_result, &kuo_minVal, &kuo_maxVal, &kuo_minLoc, &kuo_maxLoc, Mat());
	kuo_matchLoc = kuo_minLoc;
	//rectangle(image_roi, cv::Rect(kuo_matchLoc, cv::Size(image_temp_kuo_word.cols, image_temp_kuo_word.rows)), Scalar(0, 0, 255), 2, 8, 0);
	//circle(image_roi, Point(kuo_matchLoc.x, kuo_matchLoc.y), 8, Scalar(100, 100, 100), 3);
	imshow("", kuo_word_result);
	Rect rect_kuo(kuo_matchLoc, cv::Size(image_temp_kuo_word.cols, image_temp_kuo_word.rows));
	Mat image_kuo_roi = image_roi(rect_kuo);


	Mat image_kuo_bin, image_kuo_gray;
	cvtColor(image_kuo_roi, image_kuo_gray, COLOR_BGR2GRAY);
	threshold(image_kuo_gray, image_kuo_bin, 100, 255, THRESH_OTSU);
	vector<vector<Point> >contours_kuo;
	vector<Vec4i> hierarchy_kuo;
	//²éÕÒÂÖÀª
	int largest_area_kuo = 0;
	int largest_contour_index_kuo = 0;

	findContours(image_kuo_bin, contours_kuo, hierarchy_kuo, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	for (int i = 0; i < contours_kuo.size(); i++)
	{
		double a = contourArea(contours_kuo[i], false); // Find the area of contour 
		if (a > largest_area_kuo)
		{
			largest_area_kuo = a;
			largest_contour_index_kuo = i;    //Store the index of largest contour 
			//drawContours(image_roi, contours, i, color, 1, 8, hierarchy);
			//drawContours(image_yuan_roi, contours, i, color, 1, 8, hierarchy);
			//Rect r = boundingRect(contours[i]);
			//rectangle(image_roi, Point(r.x+ yuan_matchLoc.x, r.y+ yuan_matchLoc.y), Point(r.br().x+yuan_matchLoc.x, r.br().y + yuan_matchLoc.y), color,1);
		}
	}
	Rect r_kuo = boundingRect(contours_kuo[largest_contour_index_kuo]);
	rectangle(image_roi, Point(r_kuo.x + kuo_matchLoc.x, r_kuo.y + kuo_matchLoc.y), Point(r_kuo.br().x + kuo_matchLoc.x, r_kuo.br().y + kuo_matchLoc.y), color, 1);
	Point top_right(r_kuo.x + kuo_matchLoc.x+r_kuo.width, r.y + yuan_matchLoc.y);
	Point bottom_right(r_kuo.x + kuo_matchLoc.x + r_kuo.width, r_chan.br().y + chan_matchLoc.y);
	//line(image_roi, bottom_left, bottom_right, color, 2);
	circle(image_roi, top_right, 8, Scalar(100, 100, 100), 3);
	
	vector<Point2f> image_pts;
	image_pts.push_back(top_left);
	image_pts.push_back(top_right);
	image_pts.push_back(bottom_left);
	image_pts.push_back(bottom_right);

	vector<Point2f> pts_dst;
	pts_dst.push_back(Point2f(0, 0));
	pts_dst.push_back(Point2f(image_roi.cols - 1, 0));
	pts_dst.push_back(Point2f(0, image_roi.rows - 1));
	pts_dst.push_back(Point2f(image_roi.cols - 1, image_roi.rows - 1));
	Mat warp_mat = findHomography(image_pts, pts_dst);
	Mat warp_dst;
	warpPerspective(image_roi, warp_dst, warp_mat, image_roi.size(), 1, 0);
	namedWindow("img_roi", WINDOW_NORMAL);
	imshow("img_roi", image_roi);
	
	//waitKey(0);
	return warp_dst;
}

Mat scale_trans_gray(Mat image_temp, Mat image_src)
{
	
	Mat gray_word_left = imread("C:/Users/test1/Desktop/Ä£°æ/gray_word_left.jpg");
	Mat gray_word_right = imread("C:/Users/test1/Desktop/Ä£°æ/gray_word_right.jpg");
	//cout << image_temp.type() << image_temp_yuan_word.type();
	Mat src_result;
	//Ä£°æÆ¥Åä
	cv::matchTemplate(image_src, image_temp, src_result, 0);
	double minVal, maxVal;
	Point minLoc, maxLoc, matchLoc;
	minMaxLoc(src_result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	matchLoc = minLoc;
	//½ØÈ¡ÓëÄ£°æÆ¥Åä³É¹¦µÄROI
	Rect rect(matchLoc, cv::Size(image_temp.cols, image_temp.rows));
	Mat gray_image_roi = image_src(rect);
	
	Mat gray_word_left_result;
	cv::matchTemplate(gray_image_roi,gray_word_left, gray_word_left_result, TM_SQDIFF);
	double gray_word_left_minVal, gray_word_left_maxVal;
	Point gray_word_left_minLoc, gray_word_left_maxLoc, gray_word_left_matchLoc;
	minMaxLoc(gray_word_left_result, &gray_word_left_minVal, &gray_word_left_maxVal, &gray_word_left_minLoc, &gray_word_left_maxLoc, Mat());
	gray_word_left_matchLoc = gray_word_left_minLoc;
	//rectangle(gray_image_roi, cv::Rect(gray_word_left_matchLoc, cv::Size(gray_word_left.cols, gray_word_left.rows)), Scalar(0, 0, 255), 2, 8, 0);
	
	Rect gray_word_left_rect(gray_word_left_matchLoc, cv::Size(gray_word_left.cols, gray_word_left.rows));
	Mat gray_word_left_roi = gray_image_roi(gray_word_left_rect);
	Mat gray_word_left_bin, gray_word_left_gray;
	cvtColor(gray_word_left_roi, gray_word_left_gray, COLOR_BGR2GRAY);
	threshold(gray_word_left_gray, gray_word_left_bin, 40, 255, THRESH_BINARY);

	Mat gray_word_left_bin_out, gray_word_left_bin_out1;
	//medianBlur(gray_word_right_bin, gray_word_right_bin_out,3);
	boxFilter(gray_word_left_bin, gray_word_left_bin_out, -1, Size(5, 5));
	imshow("test_bin", gray_word_left_bin);
	//Mat element_left = getStructuringElement(MORPH_RECT, Size(3, 3));
	//erode(gray_word_left_bin_out, gray_word_left_bin_out1, element_left);

	vector<vector<Point> >gray_word_left_contours;
	vector<Vec4i> gray_word_left_hierarchy;
	//²éÕÒÂÖÀª
	Scalar color(rand() & 255, rand() & 255, rand() & 255);
	findContours(gray_word_left_bin_out, gray_word_left_contours, gray_word_left_hierarchy, RETR_CCOMP, CHAIN_APPROX_NONE);
	sort(gray_word_left_contours.begin(), gray_word_left_contours.end(), ContoursSortFun);
	Mat gray_word_left_temp = Mat::zeros(gray_word_left_bin.size(), gray_word_left_bin.type());
	Mat gray_word_left_temp_harr;
	drawContours(gray_word_left_temp, gray_word_left_contours, 0, color, 1, 8, gray_word_left_hierarchy);
	
	vector<Point2f> corners;
	goodFeaturesToTrack(gray_word_left_temp,corners,4,0.1,50,Mat(),3,false,0.04);
	sort(corners.begin(), corners.end(),cmpy_left);
	/*for (int i = 0; i < corners.size(); i++)
	{
		circle(gray_word_left_roi, corners[i], 2, Scalar(100, 100, 100), 3);
	}*/

	Point top_left_temp = corners[0].y < corners[1].y ? corners[0] : corners[1];
	Point dottom_left_temp = corners[0].y > corners[1].y ? corners[0] : corners[1];
		
	//circle(gray_word_left_temp, dottom_left_temp, 4, Scalar(255, 255, 255), 2, 8, 0);
	//rectangle(gray_word_left_roi, Point(r.x + gray_word_left_matchLoc.x, r.y + gray_word_left_matchLoc.y), Point(r.br().x + gray_word_left_matchLoc.x, r.br().y + gray_word_left_matchLoc.y), color, 1);
	//×óÉÏ½Ç
	Point top_left(top_left_temp.x + gray_word_left_matchLoc.x, top_left_temp.y + gray_word_left_matchLoc.y);
	//×óÏÂ½Ç
	Point dottom_left(dottom_left_temp.x + gray_word_left_matchLoc.x, dottom_left_temp.y + gray_word_left_matchLoc.y);
	///////////////
	/*±³ÃæÓÒ²à  ¡°½´ÏãÖ®ÑÅ¡±*/
	Mat gray_word_right_result;
	cv::matchTemplate(gray_image_roi, gray_word_right, gray_word_right_result, TM_SQDIFF);
	double gray_word_right_minVal, gray_word_right_maxVal;
	Point gray_word_right_minLoc, gray_word_right_maxLoc, gray_word_right_matchLoc;
	minMaxLoc(gray_word_right_result, &gray_word_right_minVal, &gray_word_right_maxVal, &gray_word_right_minLoc, &gray_word_right_maxLoc, Mat());
	gray_word_right_matchLoc = gray_word_right_minLoc;
	//rectangle(gray_image_roi, cv::Rect(gray_word_left_matchLoc, cv::Size(gray_word_left.cols, gray_word_left.rows)), Scalar(0, 0, 255), 2, 8, 0);

	Rect gray_word_right_rect(gray_word_right_matchLoc, cv::Size(gray_word_right.cols, gray_word_right.rows));
	Mat gray_word_right_roi = gray_image_roi(gray_word_right_rect);
	Mat gray_word_right_bin, gray_word_right_gray;
	cvtColor(gray_word_right_roi, gray_word_right_gray, COLOR_BGR2GRAY);
	
	threshold(gray_word_right_gray, gray_word_right_bin, 42, 255, THRESH_BINARY);
	Mat gray_word_right_bin_out, gray_word_right_bin_out1;
	//medianBlur(gray_word_right_bin, gray_word_right_bin_out,3);
	boxFilter(gray_word_right_bin, gray_word_right_bin_out, -1, Size(5,5));

	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	erode(gray_word_right_bin_out, gray_word_right_bin_out1, element);

	vector<vector<Point> >gray_word_right_contours;
	vector<Vec4i> gray_word_right_hierarchy;
	//²éÕÒÂÖÀª

	findContours(gray_word_right_bin_out1, gray_word_right_contours, gray_word_right_hierarchy, RETR_CCOMP, CHAIN_APPROX_NONE);
	sort(gray_word_right_contours.begin(), gray_word_right_contours.end(), ContoursSortFun);
	Mat gray_word_right_temp = Mat::zeros(gray_word_right_bin.size(), gray_word_right_bin.type());
	drawContours(gray_word_right_temp, gray_word_right_contours, 0, color, 1, 8, gray_word_right_hierarchy);

	vector<Point2f> right_corners;
	goodFeaturesToTrack(gray_word_right_temp, right_corners, 4, 0.05, 50, Mat(), 5, false, 0.04);
	sort(right_corners.begin(), right_corners.end(), cmpy_right_x);
	//int top_right_temp_x = right_corners[0].x;
	//sort(right_corners.begin(), right_corners.end(), cmpy_right_y);
	for (int i = 0; i < right_corners.size(); i++)
	{
		circle(gray_word_right_temp, right_corners[i],1, Scalar(100,100,100), 3);
		//imshow("1", gray_word_right_roi);
	}
	//int top_right_temp_y = right_corners[3].y;
	//int dottom_right_temp_y = right_corners[0].y;
	Point top_right_temp = right_corners[0].y < right_corners[1].y ? right_corners[0] : right_corners[1];
	Point dottom_right_temp = right_corners[0].y > right_corners[1].y ? right_corners[0] : right_corners[1];

	//ÓÒÉÏ½Ç
	Point top_right(top_right_temp.x + gray_word_right_matchLoc.x, top_right_temp.y + gray_word_right_matchLoc.y);
	//ÓÒÏÂ½Ç
	Point dottom_right(dottom_right_temp.x + gray_word_right_matchLoc.x, dottom_right_temp.y + gray_word_right_matchLoc.y);

	circle(gray_word_right_temp, dottom_right_temp, 8, Scalar(100, 100, 100), 3);
	namedWindow("tranrigh", WINDOW_NORMAL);
	imshow("tranrigh", gray_word_right_bin_out1);
	
	//
	//
	////circle(gray_word_right_roi, dottom_right_temp, 8, Scalar(100, 100, 100), 3);
	////namedWindow("img_roi", WINDOW_NORMAL);
	////imshow("img_roi", gray_word_right_roi);
	////imshow("", gray_word_left_temp);
	
	vector<Point2f> image_pts;
	image_pts.push_back(top_left);
	image_pts.push_back(top_right);
	image_pts.push_back(dottom_left);
	image_pts.push_back(dottom_right);

	vector<Point2f> pts_dst;
	pts_dst.push_back(Point2f(0, 0));
	pts_dst.push_back(Point2f(gray_image_roi.cols - 1, 0));
	pts_dst.push_back(Point2f(0, gray_image_roi.rows - 1));
	pts_dst.push_back(Point2f(gray_image_roi.cols - 1, gray_image_roi.rows - 1));
	Mat warp_mat = findHomography(image_pts, pts_dst);
	Mat warp_dst;
	warpPerspective(gray_image_roi, warp_dst, warp_mat, gray_image_roi.size(), 1, 0);
	
	//imshow("", warp_dst);
	//waitKey(0);
	return warp_dst;
}
Mat scale_trans_gray(Mat image_temp)
{

	Mat gray_word_left = imread("C:/Users/test1/Desktop/Ä£°æ/gray_word_left.jpg");
	Mat gray_word_right = imread("C:/Users/test1/Desktop/Ä£°æ/gray_word_right.jpg");
	Mat gray_image_roi = image_temp;

	Mat gray_word_left_result;
	cv::matchTemplate(gray_image_roi, gray_word_left, gray_word_left_result, TM_SQDIFF);
	double gray_word_left_minVal, gray_word_left_maxVal;
	Point gray_word_left_minLoc, gray_word_left_maxLoc, gray_word_left_matchLoc;
	minMaxLoc(gray_word_left_result, &gray_word_left_minVal, &gray_word_left_maxVal, &gray_word_left_minLoc, &gray_word_left_maxLoc, Mat());
	gray_word_left_matchLoc = gray_word_left_minLoc;
	//rectangle(gray_image_roi, cv::Rect(gray_word_left_matchLoc, cv::Size(gray_word_left.cols, gray_word_left.rows)), Scalar(0, 0, 255), 2, 8, 0);

	Rect gray_word_left_rect(gray_word_left_matchLoc, cv::Size(gray_word_left.cols, gray_word_left.rows));
	Mat gray_word_left_roi = gray_image_roi(gray_word_left_rect);
	Mat gray_word_left_bin, gray_word_left_gray;
	cvtColor(gray_word_left_roi, gray_word_left_gray, COLOR_BGR2GRAY);
	threshold(gray_word_left_gray, gray_word_left_bin, 100, 255, THRESH_OTSU);

	vector<vector<Point> >gray_word_left_contours;
	vector<Vec4i> gray_word_left_hierarchy;
	//²éÕÒÂÖÀª
	Scalar color(rand() & 255, rand() & 255, rand() & 255);
	findContours(gray_word_left_bin, gray_word_left_contours, gray_word_left_hierarchy, RETR_CCOMP, CHAIN_APPROX_NONE);
	sort(gray_word_left_contours.begin(), gray_word_left_contours.end(), ContoursSortFun);
	
	Mat gray_word_left_temp = Mat::zeros(gray_word_left_bin.size(), gray_word_left_bin.type());
	Mat gray_word_left_temp_harr;
	drawContours(gray_word_left_temp, gray_word_left_contours, 0, color, 1, 8, gray_word_left_hierarchy);

	vector<Point2f> corners;
	goodFeaturesToTrack(gray_word_left_temp, corners, 4, 0.001, 10, Mat(), 3, false, 0.04);
	sort(corners.begin(), corners.end(), cmpy_left);
	for (int i = 0; i < corners.size(); i++)
	{
		//circle(gray_word_left_roi, corners[i], 8, Scalar(100,100,100), 3);
		//imshow("1", gray_word_left_temp);
	}

	Point top_left_temp = corners[0].y < corners[1].y ? corners[0] : corners[1];
	Point dottom_left_temp = corners[0].y > corners[1].y ? corners[0] : corners[1];

	//circle(gray_word_left_temp, dottom_left_temp, 4, Scalar(255, 255, 255), 2, 8, 0);
	//rectangle(gray_word_left_roi, Point(r.x + gray_word_left_matchLoc.x, r.y + gray_word_left_matchLoc.y), Point(r.br().x + gray_word_left_matchLoc.x, r.br().y + gray_word_left_matchLoc.y), color, 1);
	//×óÉÏ½Ç
	Point top_left(top_left_temp.x + gray_word_left_matchLoc.x, top_left_temp.y + gray_word_left_matchLoc.y);
	//×óÏÂ½Ç
	Point dottom_left(dottom_left_temp.x + gray_word_left_matchLoc.x, dottom_left_temp.y + gray_word_left_matchLoc.y);
	//imshow("1", gray_word_left_roi);
	//circle(gray_word_left_temp, top_left_temp, 4, Scalar(255, 255, 255), 2, 8, 0);
	//imshow("2", gray_word_left_temp);
	
	///////////////
	Mat gray_word_right_result;
	cv::matchTemplate(gray_image_roi, gray_word_right, gray_word_right_result, TM_SQDIFF);
	double gray_word_right_minVal, gray_word_right_maxVal;
	Point gray_word_right_minLoc, gray_word_right_maxLoc, gray_word_right_matchLoc;
	minMaxLoc(gray_word_right_result, &gray_word_right_minVal, &gray_word_right_maxVal, &gray_word_right_minLoc, &gray_word_right_maxLoc, Mat());
	gray_word_right_matchLoc = gray_word_right_minLoc;
	//rectangle(gray_image_roi, cv::Rect(gray_word_left_matchLoc, cv::Size(gray_word_left.cols, gray_word_left.rows)), Scalar(0, 0, 255), 2, 8, 0);

	Rect gray_word_right_rect(gray_word_right_matchLoc, cv::Size(gray_word_right.cols, gray_word_right.rows));
	Mat gray_word_right_roi = gray_image_roi(gray_word_right_rect);
	Mat gray_word_right_bin, gray_word_right_gray;
	cvtColor(gray_word_right_roi, gray_word_right_gray, COLOR_BGR2GRAY);
	threshold(gray_word_right_gray, gray_word_right_bin, 42, 255, THRESH_BINARY);
	Mat gray_word_right_bin_out, gray_word_right_bin_out1;
	//medianBlur(gray_word_right_bin, gray_word_right_bin_out,3);
	boxFilter(gray_word_right_bin, gray_word_right_bin_out, -1, Size(3,3));
	
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5)); 
	erode(gray_word_right_bin_out, gray_word_right_bin_out1, element);
	
	vector<vector<Point> >gray_word_right_contours;
	vector<Vec4i> gray_word_right_hierarchy;
	//²éÕÒÂÖÀª

	findContours(gray_word_right_bin_out1, gray_word_right_contours, gray_word_right_hierarchy, RETR_CCOMP, CHAIN_APPROX_NONE);
	sort(gray_word_right_contours.begin(), gray_word_right_contours.end(), ContoursSortFun);
	Mat gray_word_right_temp = Mat::zeros(gray_word_right_bin.size(), gray_word_right_bin.type());
	drawContours(gray_word_right_temp, gray_word_right_contours, 0, color, 1, 8, gray_word_right_hierarchy);

	vector<Point2f> right_corners;
	goodFeaturesToTrack(gray_word_right_temp, right_corners, 4, 0.01, 50, Mat(), 3, false, 0.04);
	sort(right_corners.begin(), right_corners.end(), cmpy_right_x);
	//int top_right_temp_x = right_corners[0].x;
	//sort(right_corners.begin(), right_corners.end(), cmpy_right_y);
	//for (int i = 0; i < right_corners.size(); i++)
	//{
	//	//circle(gray_word_right_roi, right_corners[i],2, Scalar(100,100,100), 3);
	//	//imshow("1", gray_word_right_roi);
	//}
	//int top_right_temp_y = right_corners[3].y;
	//int dottom_right_temp_y = right_corners[0].y;
	Point top_right_temp = right_corners[2].y < right_corners[1].y ? right_corners[2] : right_corners[1];
	Point dottom_right_temp = right_corners[2].y > right_corners[1].y ? right_corners[2] : right_corners[1];

	//ÓÒÉÏ½Ç
	Point top_right(top_right_temp.x + gray_word_right_matchLoc.x, top_right_temp.y + gray_word_right_matchLoc.y);
	//ÓÒÏÂ½Ç
	Point dottom_right(dottom_right_temp.x + gray_word_right_matchLoc.x, dottom_right_temp.y + gray_word_right_matchLoc.y);



	circle(gray_image_roi, dottom_right, 8, Scalar(100, 100, 100), 3);
	namedWindow("tranrigh", WINDOW_NORMAL);
	imshow("tranrigh", gray_image_roi);
	

	vector<Point2f> image_pts;
	image_pts.push_back(top_left);
	image_pts.push_back(top_right);
	image_pts.push_back(dottom_left);
	image_pts.push_back(dottom_right);

	vector<Point2f> pts_dst;
	pts_dst.push_back(Point2f(0, 0));
	pts_dst.push_back(Point2f(gray_image_roi.cols - 1, 0));
	pts_dst.push_back(Point2f(0, gray_image_roi.rows - 1.0));
	pts_dst.push_back(Point2f(gray_image_roi.cols - 1.0, gray_image_roi.rows - 1.0));
	Mat warp_mat = findHomography(image_pts, pts_dst);
	Mat warp_dst;
	warpPerspective(gray_image_roi, warp_dst, warp_mat, gray_image_roi.size(), 1, 0);

	//imshow("", warp_dst);
	//waitKey(0);
	return warp_dst;
}