//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include<opencv2/xfeatures2d/nonfree.hpp>
//#include<opencv2/xfeatures2d.hpp>
//#include<opencv2/imgproc.hpp>
//#include<opencv2/opencv.hpp>
//#include<opencv2/calib3d.hpp>
//#include<iostream>
//
//using namespace std;
//using namespace cv;
//using namespace cv::xfeatures2d;
//int main()
//{
//	Mat image1 = imread("D:/酒瓶检测/线阵酒瓶/Camera MV-SUA2000M#0001-0020-beimian_20190903112103-610218196635.BMP");
//	Mat image_temp = imread("C:/Users/test1/Desktop/模版/7.bmp");
//	Mat result;
//	//模版匹配
//	cv::matchTemplate(image1, image_temp, result,TM_SQDIFF);
//	double minVal, maxVal;
//	Point minLoc, maxLoc, matchLoc;
//	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
//	matchLoc = minLoc;
//	rectangle(image1, cv::Rect(matchLoc, cv::Size(image_temp.cols, image_temp.rows)), Scalar(0, 0, 255), 2, 8, 0);
//	//截取与模版匹配成功的ROI
//	Rect rect(matchLoc, cv::Size(image_temp.cols, image_temp.rows));
//	Mat image_roi = image1(rect);
//
//	namedWindow("img_temp", WINDOW_NORMAL);
//	namedWindow("imgroi", WINDOW_NORMAL);
//	
//	imshow("imgroi", image_roi);
//	imshow("img_temp", image_temp);
//	
//	
//	 //检测surf特征点
//	vector<KeyPoint> keypoints1, keypoints2;
//
//	Ptr<SIFT> detector = SIFT::create(400);
//	detector->detect(image_temp, keypoints1);
//	detector->detect(image_roi, keypoints2);
//	// 绘制特征点
//	Mat img_keypoints_1; Mat img_keypoints_2;
//	drawKeypoints(image_temp, keypoints1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
//	drawKeypoints(image_roi, keypoints2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
//
//	// 计算特征向量
//	Ptr<SIFT>extractor = SIFT::create();
//	Mat descriptors1, descriptors2;
//	extractor->compute(image_temp, keypoints1, descriptors1);
//	extractor->compute(image_roi, keypoints2, descriptors2);
//	// 匹配
//	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");
//	std::vector< DMatch > matches;
//	matcher->match(descriptors1, descriptors2, matches);
//	
//	//提取强特征点
//	double minMatch = 1;
//	double maxMatch = 0;
//	for (int i = 0; i < matches.size(); i++)
//	{
//		//匹配值最大最小值获取
//		minMatch = minMatch > matches[i].distance ? matches[i].distance : minMatch;
//		maxMatch = maxMatch < matches[i].distance ? matches[i].distance : maxMatch;
//	}
//	////最大最小值输出
//	//cout << "最佳匹配值是： " << minMatch << endl;
//	//cout << "最差匹配值是： " << maxMatch << endl;
//
//	//获取排在前边的几个最优匹配结果
//	sort(matches.begin(), matches.end());
//
//	vector<DMatch> goodMatchePoints;
//	static int k = 0;
//	for (int i = 0; i <= 10; i++)
//	{
//		if (matches[i].distance <= max(2*minMatch,30.0))
//		{
//			goodMatchePoints.push_back(matches[i]);
//			if (goodMatchePoints.size() > 1)
//			{
//				k = k + 1;
//				if (keypoints1[goodMatchePoints[k-1.0].queryIdx].pt.x == keypoints1[goodMatchePoints[k].queryIdx].pt.x)
//				{
//					goodMatchePoints.pop_back();
//					k = k - 1;
//				}
//			}		
//		}
//	}
//	Mat imageOutput;
//	drawMatches(image_temp, keypoints1, image_roi, keypoints2, goodMatchePoints, imageOutput, Scalar::all(-1),
//		Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
//	namedWindow("add1", WINDOW_NORMAL);
//	imshow("add1", imageOutput);
//	////提取最佳匹配点
//	Point2f srcTri[3], dstTri[3];
//	//Mat warp_mat(2, 3, CV_32FC1);
//	Mat src, warp_dst, warp_rotate_dst,canny_image,add_image,image2_gray,warp_gray,warp_bin;
//	for (int i = 0; i <= 2; i++)
//	{
//		srcTri[i].x = keypoints1[goodMatchePoints[i].queryIdx].pt.x;
//		srcTri[i].y = keypoints1[goodMatchePoints[i].queryIdx].pt.y;
//		dstTri[i].x = keypoints2[goodMatchePoints[i].trainIdx].pt.x;
//		dstTri[i].y = keypoints2[goodMatchePoints[i].trainIdx].pt.y;
//		cout << keypoints1[goodMatchePoints[i].queryIdx].pt.x <<" "<< keypoints1[goodMatchePoints[i].queryIdx].pt.y<<" " <<keypoints2[goodMatchePoints[i].trainIdx].pt.x<<" "<< keypoints2[goodMatchePoints[i].trainIdx].pt.y<<endl;
//	}
//	//Mat warp_mat = getPerspectiveTransform(srcTri, dstTri);
//	//warpPerspective(image_temp, warp_dst, warp_mat, image_temp.size(), 1, 0);
//	Mat warp_mat = getAffineTransform(srcTri, dstTri);
//	warpAffine(image_temp, warp_dst, warp_mat, image_roi.size(),1,0);
//	cout << warp_mat << endl;
//	//namedWindow("warp", WINDOW_NORMAL);
//	//imshow("warp", warp_dst); 
//
//	cvtColor(warp_dst, warp_gray, COLOR_BGR2GRAY);
//	threshold(warp_gray, warp_bin, 100, 255, THRESH_OTSU);
//	bitwise_not(warp_bin, warp_bin);
//	Canny(warp_bin, canny_image, 3, 39, 3);
//	vector<vector<Point> >contours;
//	vector<Vec4i> hierarchy;
//	//查找轮廓
//	cout << warp_gray.type();
//	findContours(warp_bin, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
//	for (int i = 0; i < contours.size(); i++)
//	{
//		Scalar color(rand() & 255, rand() & 255, rand() & 255);
//		drawContours(image_roi, contours, i, color, 1, 8, hierarchy);
//		drawContours(image_temp, contours, i, color, 1, 8, hierarchy);
//	}
//	//addWeighted(image_roi, 0.9, canny_image, 0.1, 3, add_image);
//
//	//cout << canny_image.size() << image2.size();
//	circle(image_temp, Point(keypoints1[goodMatchePoints[2].queryIdx].pt.x, keypoints1[goodMatchePoints[2].queryIdx].pt.y), 3, Scalar(0, 0, 0));
//	circle(image_roi, Point(keypoints2[goodMatchePoints[2].trainIdx].pt.x, keypoints2[goodMatchePoints[2].trainIdx].pt.y), 3, Scalar(0, 0, 0));
//	//绘制最优匹配点
//	/*Mat imageOutput;
//	drawMatches(image1, keypoints1, image2, keypoints2, goodMatchePoints, imageOutput, Scalar::all(-1),
//		Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
//	*/
//	
//	//Mat imgMatches;
//	//drawMatches(image1, keypoints1, image2, keypoints2, matches, imgMatches, DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
//	////优化匹配点
//
//	namedWindow("add", WINDOW_NORMAL);
//	namedWindow("imageroi", WINDOW_NORMAL);
//	namedWindow("warp",WINDOW_NORMAL);
//	imshow("imageroi",image_roi);
//	imshow("warp", warp_dst);
//	imshow("add", image_temp);
//	waitKey(0);
//
//	return 0;
//}
//
