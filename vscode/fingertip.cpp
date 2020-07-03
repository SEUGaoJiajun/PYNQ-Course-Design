/*
 * @Date: 2020-05-21 10:46:04
 * @LastEditors: LHD
 * @LastEditTime: 2020-06-19 19:58:59
 * @FilePath: \c++exercise\exam2Main.cpp
 * @Description: 
 */ 

//指尖检测
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <opencv2\opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/features2d/features2d.hpp>
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;
void KeyPointsToPoints(vector<KeyPoint> kpts, vector<Point2f> &pts)
{
	for (int i = 0; i < kpts.size(); i++) {
		pts.push_back(kpts[i].pt);
	}
}
void skinExtract(const Mat &frame, Mat1b &skinArea); //肤色提取，skinArea为二值化肤色图像
int main(int argc, char **argv)
{
	//string nemePicture("12.jpg");
	Mat im = imread("12.jpg");
	Mat test = imread("hand.jpg");
	Mat dst(test.rows, test.cols, CV_8UC1);
	Mat g_grayImage;
	//肤色提取
	Mat1b skinArea(im.rows, im.cols);
	cvtColor(test, g_grayImage, COLOR_BGR2GRAY);
	cornerHarris(g_grayImage, dst, 2, 3, 0.04, BORDER_DEFAULT);
    //imshow("显示图",dst);waitKey();

	skinExtract(im, skinArea);

	// Draw contours + hull results
	Mat drawing = Mat::zeros(im.size(), CV_8UC3);
	im.copyTo(drawing);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	//寻找轮廓
	findContours(skinArea, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

	// 找到最大的轮廓（手掌轮廓）
	int index;
	double area, maxArea(0);
	for (int i = 0; i < contours.size(); i++)
	{
		area = contourArea(Mat(contours[i]));
		if (area > maxArea)
		{
			maxArea = area;
			index = i;
		}
	}
	drawContours(drawing, contours, index, Scalar(0, 0, 255), 2, 8, hierarchy);
	//imshow("显示图",drawing);waitKey();
	//计算重心，使用别的函数代替
	Point center1;
	//vector<Point> couPoint;
	int height = im.rows;
	int width = im.cols;
	int sum_x = 0, sum_y = 0, areas = 0, n=0;
	Point a[709];
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int num = i * width + j;

			int b1 = (int)test.data[3 * num + 0];
			int g1 = (int)test.data[3 * num + 1];
			int r1 = (int)test.data[3 * num + 2];

			if (b1 == 255 && g1 == 255 & r1 == 255)
			{
				sum_x = sum_x + i; //计算第Ｋ区域的横坐标总和
				sum_y = sum_y + j; //计算第Ｋ区域的纵坐标总和
				//a[areas].x = j;
				//a[areas].y = i;
				areas = areas + 1;
				// couPoint[n].y = i;
				// couPoint[n].x = j;
				// n++;
				
			}
		}
	}
	center1.x = round(sum_x / areas);
	center1.y = round(sum_y / areas);
	//copy(a, a + areas, couPoint.begin());

	Moments moment = moments(skinArea, true);
	Point center(moment.m10 / moment.m00, moment.m01 / moment.m00);
	// center.x = center1.y;
	// center.y = center1.x;
	circle(drawing, center, 8, Scalar(0, 0, 255), -1);
	cout << center.x << '\t' << center.y << endl;
	cout << center1.x << '\t' << center1.y<<endl;
	
	// 寻找指尖
    vector<KeyPoint> keypoints;  
	FAST(im, keypoints, 60,true);
	Mat image;
	drawKeypoints(im, keypoints, image, Scalar::all(-1),DrawMatchesFlags::DEFAULT);
	//imshow("FAST feature", image);waitKey();
	//vector<Point2f> couPoint;
	//KeyPoint::convert(keypoints,couPoint);
	//KeyPointsToPoints(keypoints, couPoint);
	vector<Point> couPoint = contours[index];
	vector<Point> fingerTips;
	Point tmp;
	int max(0), count(0), notice(0);
	for (int i = 0; i < couPoint.size(); i++)
	{
		tmp = couPoint[i];
		int dist = (tmp.x - center.x) * (tmp.x - center.x) + (tmp.y - center.y) * (tmp.y - center.y);
		if (dist > max)
		{
			max = dist;
			notice = i;
		}

		// 计算最大值保持的点数，如果大于40（这个值需要设置，本来想根据max值来设置，
		// 但是不成功，不知道为何），那么就认为这个是指尖
		if (dist != max)
		{
			count++;
			if (count > 40)
			{
				count = 0;
				max = 0;
				bool flag = false;
				// 低于手心的点不算
				if (center.y < couPoint[notice].y)
					continue;
				// 离得太近的不算
				for (int j = 0; j < fingerTips.size(); j++)
				{
					if (abs(couPoint[notice].x - fingerTips[j].x) < 20)
					{
						flag = true;
						break;
					}
				}
				if (flag)
					continue;
				fingerTips.push_back(couPoint[notice]);
				circle(drawing, couPoint[notice], 6, Scalar(0, 255, 0), -1);
				line(drawing, center, couPoint[notice], Scalar(255, 0, 0), 2);
			}
		}
	}
	cout << fingerTips.size();
	namedWindow("helloCV");
	imshow("helloCV", drawing);
	waitKey();
	imwrite("result12.png", drawing);
	destroyWindow("helloCV");
	printf("hello\n");
	return 0;
}

void skinExtract(const Mat &im, Mat1b &skinArea)
{
	//转换为YCrCb颜色空间
	Mat YCrCb;
	cvtColor(im, YCrCb, CV_BGR2YCrCb);

	//将多通道图像分离为多个单通道图像
	vector<Mat> planes;
	split(YCrCb, planes);

	Mat m0 = planes[0], m1 = planes[1], m2 = planes[2];

	//运用迭代器访问矩阵元素
	MatIterator_<uchar> it_Cb = planes[1].begin<uchar>(), it_Cb_end = planes[1].end<uchar>();
	MatIterator_<uchar> it_Cr = planes[2].begin<uchar>();
	MatIterator_<uchar> it_skin = skinArea.begin();

	//人的皮肤颜色在YCbCr色度空间的分布范围:130<=Cb<=170, 85<=Cr<=125
	for (; it_Cb != it_Cb_end; ++it_Cr, ++it_Cb, ++it_skin)
	{
		if (85 <= *it_Cr && *it_Cr <= 125 && 130 <= *it_Cb && *it_Cb <= 170)
			*it_skin = 255;
		else
		{
			*it_skin = 0;
		}
	}
	medianBlur(skinArea, skinArea, 3);

	//膨胀和腐蚀，膨胀可以填补凹洞（将裂缝桥接），腐蚀可以消除细的凸起（“斑点”噪声）
	dilate(skinArea, skinArea, Mat(5, 5, CV_8UC1), Point(-1, -1));
	erode(skinArea, skinArea, Mat(5, 5, CV_8UC1), Point(-1, -1));
}