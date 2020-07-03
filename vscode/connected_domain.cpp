/*
 * @Date: 2020-06-15 15:24:41
 * @LastEditors: LHD
 * @LastEditTime: 2020-06-19 19:58:27
 * @FilePath: \c++exercise\liantong.cpp
 * @Description: 
 */ 

#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>
#include <stack>
using namespace std;
using namespace cv;

typedef struct _Feather
{
    int label;        // 连通域的label值
    int area;         // 连通域的面积
    Rect boundingbox; // 连通域的外接矩形框
} Feather;
int labelmax, areamax = 0;
int count[400][300];

/**
 * @description: 求连通域标签
 * @param {type} 
   Input: 
    src: 待检测连通域的二值化图像
   Output:
    dst: 标记后的图像
    featherList: 连通域特征的清单
 * @return: 连通域数量。
 */
int bwLabel(Mat &src, Mat &dst, vector<Feather> &featherList, int num[][300])
{
    int rows = src.rows;
    int cols = src.cols;

    int labelValue = 0;
    Point seed, neighbor;
    stack<Point> pointStack; // 堆栈

    int area = 0;         // 用于计算连通域的面积
    int leftBoundary = 0; // 连通域的左边界，即外接最小矩形的左边框，横坐标值，依此类推
    int rightBoundary = 0;
    int topBoundary = 0;
    int bottomBoundary = 0;
    Rect box; // 外接矩形框
    Feather feather;

    featherList.clear(); // 清除数组

    dst.release();
    dst = src.clone();

    for (int i = 0; i < rows; i++)
    {
        //uchar *pRow = dst.ptr<uchar>(i);
        for (int j = 0; j < cols; j++)
        {
            num[j][i] = dst.at<uchar>(i, j);
            if (dst.at<uchar>(i, j) == 255)
            {
                area = 0;
                labelValue++;       // labelValue最大为254，最小为1.
                seed = Point(j, i); // Point（横坐标，纵坐标）
                dst.at<uchar>(seed) = labelValue;
                num[j][i] = labelValue;
                pointStack.push(seed);

                area++;
                leftBoundary = seed.x;
                rightBoundary = seed.x;
                topBoundary = seed.y;
                bottomBoundary = seed.y;

                while (!pointStack.empty())
                {
                    neighbor = Point(seed.x + 1, seed.y);
                    if ((seed.x != (cols - 1)) && (dst.at<uchar>(neighbor) == 255))
                    {
                        dst.at<uchar>(neighbor) = labelValue;
                        pointStack.push(neighbor);

                        area++;
                        if (rightBoundary < neighbor.x)
                            rightBoundary = neighbor.x;
                    }

                    neighbor = Point(seed.x, seed.y + 1);
                    if ((seed.y != (rows - 1)) && (dst.at<uchar>(neighbor) == 255))
                    {
                        dst.at<uchar>(neighbor) = labelValue;
                        pointStack.push(neighbor);

                        area++;
                        if (bottomBoundary < neighbor.y)
                            bottomBoundary = neighbor.y;
                    }

                    neighbor = Point(seed.x - 1, seed.y);
                    if ((seed.x != 0) && (dst.at<uchar>(neighbor) == 255))
                    {
                        dst.at<uchar>(neighbor) = labelValue;
                        pointStack.push(neighbor);

                        area++;
                        if (leftBoundary > neighbor.x)
                            leftBoundary = neighbor.x;
                    }

                    neighbor = Point(seed.x, seed.y - 1);
                    if ((seed.y != 0) && (dst.at<uchar>(neighbor) == 255))
                    {
                        dst.at<uchar>(neighbor) = labelValue;
                        pointStack.push(neighbor);

                        area++;
                        if (topBoundary > neighbor.y)
                            topBoundary = neighbor.y;
                    }

                    seed = pointStack.top();
                    pointStack.pop();
                }
                box = Rect(leftBoundary, topBoundary, rightBoundary - leftBoundary, bottomBoundary - topBoundary);
                rectangle(src, box, 255);
                feather.area = area;
                feather.boundingbox = box;
                feather.label = labelValue;
                featherList.push_back(feather);
                //  num[i][j] = labelValue;
            }
        }
    }
    return labelValue;
}

int main(int argc, char *argv[])
{
    Mat src(imread("liantong.jpg", 0));
    threshold(src, src, 127, 255, THRESH_BINARY); // 二值化图像
    vector<Feather> featherList;                  // 存放连通域特征
    Mat dst;
    int num[400][300];
    cout << "number：" << bwLabel(src, dst, featherList, num) << endl;

    // cout << "label" << "\t" << "area" << endl;
    for (vector<Feather>::iterator it = featherList.begin(); it < featherList.end(); it++)
    {

        cout << it->label << "\t" << it->area << endl;
        if (it->area > areamax)
        {
            labelmax = it->label;
            areamax = it->area;
        }
        rectangle(dst, it->boundingbox, 255);
    }

    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            //cout << num[i][j];
            if (dst.at<uchar>(i, j) != labelmax)
                dst.at<uchar>(i, j) = 0;
            else
                dst.at<uchar>(i, j) = 255;
        }
        // cout << endl;
    }

    imshow("src", src);
    imshow("dst", dst);

    waitKey();
    //destroyAllWindows();

    system("pause");
    return 0;
}
