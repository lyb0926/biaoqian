#include"pipei.hpp"

void pipei1(Mat image_temp, Mat image_roi)
{
	

	vector<KeyPoint> keypoints1, keypoints2;
	Ptr<SIFT> detector = SIFT::create(900);
	detector->detect(image_temp, keypoints2);
	detector->detect(image_roi, keypoints1);
	// 绘制特征点
	Mat img_keypoints_1; Mat img_keypoints_2;
	drawKeypoints(image_temp, keypoints2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	drawKeypoints(image_roi, keypoints1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

	// 计算特征向量
	Ptr<SIFT>extractor = SIFT::create();
	Mat descriptors1, descriptors2;
	extractor->compute(image_temp, keypoints2, descriptors2);
	extractor->compute(image_roi, keypoints1, descriptors1);
	// 匹配
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");
	std::vector< DMatch > matches;
	matcher->match(descriptors1, descriptors2, matches);
	//提取强特征点
	sort(matches.begin(), matches.end());
	/*
	取前10特征点，排除重复特征点
	*/
	vector<DMatch> goodMatchePoints;
	static int k = 0;
	for (int i = 0; i <= 150; i++)
	{
		goodMatchePoints.push_back(matches[i]);
		if (goodMatchePoints.size() > 0)
		{
			k = k + 1;
			if (abs(keypoints1[goodMatchePoints[k-1.0].queryIdx].pt.x - keypoints2[goodMatchePoints[k-1.0].trainIdx].pt.x) > 5 ||
				abs(keypoints1[goodMatchePoints[k-1.0].queryIdx].pt.y - keypoints2[goodMatchePoints[k-1.0].trainIdx].pt.y) > 5)
			{
				int a = abs(keypoints1[goodMatchePoints[k-1.0].queryIdx].pt.x - keypoints2[goodMatchePoints[k-1.0].trainIdx].pt.x);
				goodMatchePoints.pop_back();
				k = k - 1;
			}	
			if (goodMatchePoints.size() > 1)
			{
				if (keypoints1[goodMatchePoints[k - 1.0].queryIdx].pt.x == keypoints1[goodMatchePoints[k-2.0].queryIdx].pt.x)
				{
					goodMatchePoints.pop_back();
					k = k - 1;
				}
			}
		}
	}

	
	//
	Point2f srcTri[3], dstTri[3];
	Mat src, warp_dst, warp_rotate_dst, canny_image, add_image, image2_gray, warp_gray, warp_bin;
	for (int i = 0; i <= 2; i++)
	{
		srcTri[i].x = keypoints1[goodMatchePoints[i].queryIdx].pt.x;
		srcTri[i].y = keypoints1[goodMatchePoints[i].queryIdx].pt.y;
		dstTri[i].x = keypoints2[goodMatchePoints[i].trainIdx].pt.x;
		dstTri[i].y = keypoints2[goodMatchePoints[i].trainIdx].pt.y;
		cout << keypoints1[goodMatchePoints[i].queryIdx].pt.x << " " << keypoints1[goodMatchePoints[i].queryIdx].pt.y << " " << keypoints2[goodMatchePoints[i].trainIdx].pt.x << " " << keypoints2[goodMatchePoints[i].trainIdx].pt.y << endl;
		circle(image_roi, Point(keypoints1[goodMatchePoints[i].queryIdx].pt.x, keypoints1[goodMatchePoints[i].queryIdx].pt.y), 8, Scalar(100, 100, 100), 3);
		circle(image_temp, Point(keypoints2[goodMatchePoints[i].trainIdx].pt.x, keypoints2[goodMatchePoints[i].trainIdx].pt.y), 8, Scalar(100, 100, 100), 3);
		imshow("imgroi2", image_roi);
		imshow("img_temp2", image_temp);
	}

	//Mat warp_mat = getPerspectiveTransform(srcTri, dstTri);
	//warpPerspective(image_roi, warp_dst, warp_mat, image_roi.size(), 1, 0);
	//Mat tran_mat = findHomography(srcTri, dstTri);
	Mat warp_mat = getAffineTransform(srcTri, dstTri);
	warpAffine(image_roi, warp_dst, warp_mat, image_roi.size(), 1, 0);
	cout << warp_mat << endl;

	cvtColor(warp_dst, warp_gray, COLOR_BGR2GRAY);
	threshold(warp_gray, warp_bin, 0, 255, THRESH_OTSU);
	namedWindow("warp_bin", WINDOW_NORMAL);
	imshow("warp_bin", warp_dst);
	bitwise_not(warp_bin, warp_bin);
	Canny(warp_bin, canny_image, 3, 39, 3);
	Mat contours_image = Mat::zeros(image_temp.size(), image_temp.type());
	Mat image_temp_gray, image_temp_bin;
	cvtColor(image_temp, image_temp_gray, COLOR_BGR2GRAY);
	threshold(image_temp_gray, image_temp_bin, 0, 255, THRESH_OTSU);
	vector<vector<Point> >contours_temp;
	vector<Vec4i> hierarchy_temp;
	//查找轮廓
	cout << warp_gray.type();
	findContours(image_temp_bin, contours_temp, hierarchy_temp, RETR_TREE, CHAIN_APPROX_NONE);
	for (int i = 0; i < contours_temp.size(); i++)
	{
		Scalar color(rand() & 255, rand() & 255, rand() & 255);
		drawContours(warp_dst, contours_temp, i, color, 0.5, 4, hierarchy_temp);
		//drawContours(image_temp, contours, i, color, 1, 8, hierarchy);
	}

	vector<vector<Point> >contours;
	vector<Vec4i> hierarchy;
	//查找轮廓

	findContours(warp_bin, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color(rand() & 255, rand() & 255, rand() & 255);
		drawContours(contours_image, contours, i, color, 0.5, 4, hierarchy);
		//drawContours(contours_image, contours_temp, i, color, 0.5, 4, hierarchy_temp);
		//drawContours(image_temp, contours, i, color, 1, 8, hierarchy);
	}
	namedWindow("add2", WINDOW_NORMAL);
	namedWindow("warp2", WINDOW_NORMAL);
	imshow("warp2", warp_dst);
	imshow("add2", contours_image);
}

void pipei2(Mat image_temp, Mat image_src)
{
	Mat result;
	//模版匹配
	cv::matchTemplate(image_src, image_temp, result, TM_SQDIFF);
	double minVal, maxVal;
	Point minLoc, maxLoc, matchLoc;
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	matchLoc = minLoc;
	rectangle(image_src, cv::Rect(matchLoc, cv::Size(image_temp.cols, image_temp.rows)), Scalar(0, 0, 255), 2, 8, 0);
	//截取与模版匹配成功的ROI
	Rect rect(matchLoc, cv::Size(image_temp.cols, image_temp.rows));
	Mat image_roi = image_src(rect);

	namedWindow("img_temp2", WINDOW_NORMAL);
	namedWindow("imgroi2", WINDOW_NORMAL);

	imshow("imgroi2", image_roi);
	imshow("img_temp2", image_temp);

	vector<KeyPoint> keypoints1, keypoints2;

	Ptr<SIFT> detector = SIFT::create(900);
	detector->detect(image_temp, keypoints2);
	detector->detect(image_roi, keypoints1);
	// 绘制特征点
	Mat img_keypoints_1; Mat img_keypoints_2;
	drawKeypoints(image_temp, keypoints2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	drawKeypoints(image_roi, keypoints1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

	// 计算特征向量
	Ptr<SIFT>extractor = SIFT::create();
	Mat descriptors1, descriptors2;
	extractor->compute(image_temp, keypoints2, descriptors2);
	extractor->compute(image_roi, keypoints1, descriptors1);
	// 匹配
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");
	std::vector< DMatch > matches;
	matcher->match(descriptors1, descriptors2, matches);
	//提取强特征点
	sort(matches.begin(), matches.end());
	/*
	取前10特征点，排除重复特征点
	*/
	vector<DMatch> goodMatchePoints;
	static int k = 0;
	for (int i = 0; i <= 10; i++)
	{
		goodMatchePoints.push_back(matches[i]);
		if (goodMatchePoints.size() > 0)
		{
			k = k + 1;
			if (abs(keypoints1[goodMatchePoints[k - 1.0].queryIdx].pt.x - keypoints2[goodMatchePoints[k - 1.0].trainIdx].pt.x) > 5 ||
				abs(keypoints1[goodMatchePoints[k - 1.0].queryIdx].pt.y - keypoints2[goodMatchePoints[k - 1.0].trainIdx].pt.y) > 5)
			{
				int a = abs(keypoints1[goodMatchePoints[k - 1.0].queryIdx].pt.x - keypoints2[goodMatchePoints[k - 1.0].trainIdx].pt.x);
				goodMatchePoints.pop_back();
				k = k - 1;
			}
			if (goodMatchePoints.size() > 1)
			{
				if (keypoints1[goodMatchePoints[k - 1.0].queryIdx].pt.x == keypoints1[goodMatchePoints[k - 2.0].queryIdx].pt.x)
				{
					goodMatchePoints.pop_back();
					k = k - 1;
				}
			}
		}
	}
	//
	Point2f srcTri[3], dstTri[3];
	Mat src, warp_dst, warp_rotate_dst, canny_image, add_image, image2_gray, warp_gray, warp_bin;
	for (int i = 0; i <=2; i++)
	{
		srcTri[i].x = keypoints1[goodMatchePoints[i].queryIdx].pt.x;
		srcTri[i].y = keypoints1[goodMatchePoints[i].queryIdx].pt.y;
		dstTri[i].x = keypoints2[goodMatchePoints[i].trainIdx].pt.x;
		dstTri[i].y = keypoints2[goodMatchePoints[i].trainIdx].pt.y;
		cout << keypoints1[goodMatchePoints[i].queryIdx].pt.x << " " << keypoints1[goodMatchePoints[i].queryIdx].pt.y << " " << keypoints2[goodMatchePoints[i].trainIdx].pt.x << " " << keypoints2[goodMatchePoints[i].trainIdx].pt.y << endl;
		circle(image_roi, Point(keypoints1[goodMatchePoints[i].queryIdx].pt.x, keypoints1[goodMatchePoints[i].queryIdx].pt.y), 8, Scalar(100, 100, 100), 3);
		circle(image_temp, Point(keypoints2[goodMatchePoints[i].trainIdx].pt.x, keypoints2[goodMatchePoints[i].trainIdx].pt.y), 8, Scalar(100, 100, 100), 3);
		imshow("imgroi2", image_roi);
		imshow("img_temp2", image_temp);
	}
	
	//Mat warp_mat = getPerspectiveTransform(srcTri, dstTri);
	//warpPerspective(image_roi, warp_dst, warp_mat, image_roi.size(), 1, 0);
	//Mat tran_mat = findHomography(srcTri, dstTri);
	Mat warp_mat = getAffineTransform(srcTri, dstTri);
	warpAffine(image_roi, warp_dst, warp_mat, image_roi.size(), 1, 0);
	cout << warp_mat << endl;

	cvtColor(warp_dst, warp_gray, COLOR_BGR2GRAY);
	threshold(warp_gray, warp_bin, 0, 255, THRESH_OTSU);
	namedWindow("warp_bin", WINDOW_NORMAL);
	imshow("warp_bin", warp_dst);
	bitwise_not(warp_bin, warp_bin);
	Canny(warp_bin, canny_image, 3, 39, 3);
	Mat contours_image = Mat::zeros(image_temp.size(), image_temp.type());
	Mat image_temp_gray,image_temp_bin;
	cvtColor(image_temp, image_temp_gray, COLOR_BGR2GRAY);
	threshold(image_temp_gray, image_temp_bin, 0, 255, THRESH_OTSU);
	vector<vector<Point> >contours_temp;
	vector<Vec4i> hierarchy_temp;
	//查找轮廓
	cout << warp_gray.type();
	findContours(image_temp_bin, contours_temp, hierarchy_temp, RETR_TREE, CHAIN_APPROX_NONE);
	for (int i = 0; i < contours_temp.size(); i++)
	{
		Scalar color(rand() & 255, rand() & 255, rand() & 255);
		drawContours(contours_image, contours_temp, i, color, 0.5, 4, hierarchy_temp);
		//drawContours(image_temp, contours, i, color, 1, 8, hierarchy);
	}
	
	vector<vector<Point> >contours;
	vector<Vec4i> hierarchy;
	//查找轮廓
	
	findContours(warp_bin, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color(rand() & 255, rand() & 255, rand() & 255);
		drawContours(contours_image, contours, i, color, 0.5, 4, hierarchy);
		//drawContours(contours_image, contours_temp, i, color, 0.5, 4, hierarchy_temp);
		//drawContours(image_temp, contours, i, color, 1, 8, hierarchy);
	}
	namedWindow("add2", WINDOW_NORMAL);
	namedWindow("warp2", WINDOW_NORMAL);
	imshow("warp2", warp_dst);
	imshow("add2", contours_image);
}

void pipei3(Mat image_temp, Mat image_src)
{
	Mat result;
	//模版匹配
	cv::matchTemplate(image_src, image_temp, result, TM_SQDIFF_NORMED);
	double minVal, maxVal;
	Point minLoc, maxLoc, matchLoc;
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	matchLoc = minLoc;
	//rectangle(image_src, cv::Rect(matchLoc, cv::Size(image_temp.cols, image_temp.rows)), Scalar(0, 0, 255), 2, 8, 0);
	//截取与模版匹配成功的ROI
	Rect rect(matchLoc, cv::Size(image_temp.cols, image_temp.rows));
	Mat image_roi = image_src(rect);

	namedWindow("img_temp3", WINDOW_NORMAL);
	namedWindow("imgroi3", WINDOW_NORMAL);

	//imshow("imgroi3", image_roi);
	//imshow("img_temp3", image_temp);

	vector<KeyPoint> keypoints1, keypoints2;

	Ptr<SIFT> detector = SIFT::create(6000);
	detector->detect(image_temp, keypoints2);
	detector->detect(image_roi, keypoints1);
	// 绘制特征点
	Mat img_keypoints_1; Mat img_keypoints_2;
	drawKeypoints(image_temp, keypoints2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	drawKeypoints(image_roi, keypoints1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

	// 计算特征向量
	Ptr<SIFT>extractor = SIFT::create();
	Mat descriptors1, descriptors2;
	extractor->compute(image_temp, keypoints2, descriptors2);
	extractor->compute(image_roi, keypoints1, descriptors1);
	// 匹配
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");
	std::vector< DMatch > matches;
	matcher->match(descriptors1, descriptors2, matches);
	//提取强特征点
	sort(matches.begin(), matches.end());
	Point2f srcTri[3], dstTri[3];
	Mat src, warp_dst, warp_rotate_dst, canny_image, add_image, image2_gray, warp_gray, warp_bin;
	for (int i = 0; i <= 2; i++)
	{
		srcTri[i].x = keypoints1[matches[i].queryIdx].pt.x;
		srcTri[i].y = keypoints1[matches[i].queryIdx].pt.y;
		dstTri[i].x = keypoints2[matches[i].trainIdx].pt.x;
		dstTri[i].y = keypoints2[matches[i].trainIdx].pt.y;
		cout << keypoints1[matches[i].queryIdx].pt.x << " " << keypoints1[matches[i].queryIdx].pt.y << " " << keypoints2[matches[i].trainIdx].pt.x << " " << keypoints2[matches[i].trainIdx].pt.y << endl;
		circle(image_roi, Point(keypoints1[matches[i].queryIdx].pt.x, keypoints1[matches[i].queryIdx].pt.y), 8, Scalar(100, 100, 100), 3);
		circle(image_temp, Point(keypoints2[matches[i].trainIdx].pt.x, keypoints2[matches[i].trainIdx].pt.y), 8, Scalar(100, 100, 100), 3);
		imshow("imgroi2", image_roi);
		imshow("img_temp2", image_temp);
	}
	/*Mat warp_mat = getPerspectiveTransform(srcTri, dstTri);
	warpPerspective(image_temp, warp_dst, warp_mat, image_roi.size(), 1, 0);*/
	Mat warp_mat = getAffineTransform(srcTri, dstTri);
	warpAffine(image_roi, warp_dst, warp_mat, image_roi.size(), 1, 0);
	cout << warp_mat << endl;

	cvtColor(warp_dst, warp_gray, COLOR_BGR2GRAY);
	threshold(warp_gray, warp_bin, 100, 255, THRESH_OTSU);
	bitwise_not(warp_bin, warp_bin);
	Canny(warp_bin, canny_image, 3, 39, 3);
	vector<vector<Point> >contours;
	vector<Vec4i> hierarchy;
	//查找轮廓
	cout << warp_gray.type();
	findContours(warp_bin, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color(rand() & 255, rand() & 255, rand() & 255);
		//drawContours(image_roi, contours, i, color, 1, 8, hierarchy);
		drawContours(image_temp, contours, i, color, 1, 8, hierarchy);
	}
	namedWindow("add3", WINDOW_NORMAL);
	namedWindow("warp3", WINDOW_NORMAL);
	imshow("warp3", warp_dst);
	imshow("add3", image_temp);
}

void temp_match(Mat src_image, Mat temp_image, Mat temp_image_out, int MODE)
{

}