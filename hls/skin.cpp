#include "top.h"
#include <string.h>
#include <iostream>
#include <math.h>
#include <stack>
using namespace std;
//vector<myPoint> couPoint(1000);
myPoint center;
int labelmax, areamax = 0;
int countnum[MAX_HEIGHT][MAX_WIDTH];
int countnum2[MAX_HEIGHT][MAX_WIDTH];
int cnt = 0,cnt2=0,cnt3=0;

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
int bwLabel(RGB_IMAGE &src, Feather featherList[])
{
	int rows = src.rows;
	int cols = src.cols;

	int labelValue = 0;
	hls::Point_<int> seed, neighbor;
	//stack<hls::Point_<int> > pointStack; // 堆栈
	hls::Point_<int>  pointStack2[50000]; // 堆栈
	for(int z=0;z<50000;z++)
	{
		pointStack2[z].x=0;
		pointStack2[z].y=0;
	}
	int area = 0;		  // 用于计算连通域的面积
	int leftBoundary = 0; // 连通域的左边界，即外接最小矩形的左边框，横坐标值，依此类推
	int rightBoundary = 0;
	int topBoundary = 0;
	int bottomBoundary = 0;
    int state=1;
	Feather feather;

	
	RGB_PIXEL curinPix;
	// GRAY_PIXEL outPix;
	for (int i = 0; i < rows; i++)
	{
			//uchar *pRow = dst.ptr<uchar>(i);
	 for (int j = 0; j < cols; j++)
		{
	        if (i < rows && j < cols)
				{
					src >> curinPix;
					countnum[i][j] = curinPix.val[0];
				}
		}
	}
	for (int i = 0; i < rows; i++)
	{
		//uchar *pRow = dst.ptr<uchar>(i);
		for (int j = 0; j < cols; j++)
		{


			if (countnum[i][j] == 255)
			{
				area = 0;
				cnt2=0;
				labelValue++;				   // labelValue最大为254，最小为1.
				seed = hls::Point_<int>(j, i); // Point（横坐标，纵坐标）
				//dst.at<uchar>(seed) = labelValue;
				countnum[i][j] = labelValue;
				//pointStack.push(seed);
				pointStack2[cnt2].x=seed.x;
				pointStack2[cnt2].y=seed.y;
				cnt2++;
				area++;
				leftBoundary = seed.x;
				rightBoundary = seed.x;
				topBoundary = seed.y;
				bottomBoundary = seed.y;
				while (pointStack2[0].x!=0 && pointStack2[0].y!=0)
				//while (!pointStack.empty())
				{
					neighbor = hls::Point_<int>(seed.x + 1, seed.y);
					if ((seed.x != (cols - 1)) && (countnum[seed.y][seed.x + 1] == 255))
					{
						countnum[seed.y][seed.x + 1] = labelValue;

				//		pointStack.push(neighbor);
						pointStack2[cnt2].x=neighbor.x;
						pointStack2[cnt2].y=neighbor.y;
						cnt2++;
						area++;
						if (rightBoundary < neighbor.x)
							rightBoundary = neighbor.x;
					}

					neighbor = hls::Point_<int>(seed.x, seed.y + 1);
					if ((seed.y != (rows - 1)) && (countnum[seed.y + 1][seed.x] == 255))
					{
						countnum[seed.y + 1][seed.x] = labelValue;

					//	pointStack.push(neighbor);
						pointStack2[cnt2].x=neighbor.x;
						pointStack2[cnt2].y=neighbor.y;
						cnt2++;
						area++;
						if (bottomBoundary < neighbor.y)
							bottomBoundary = neighbor.y;
					}

					neighbor = hls::Point_<int>(seed.x - 1, seed.y);
					if ((seed.x != 0) && (countnum[seed.y][seed.x - 1] == 255))
					{
						countnum[seed.y][seed.x - 1] = labelValue;

					//	pointStack.push(neighbor);
						pointStack2[cnt2].x=neighbor.x;
						pointStack2[cnt2].y=neighbor.y;
						cnt2++;
						area++;
						if (leftBoundary > neighbor.x)
							leftBoundary = neighbor.x;
					}

					neighbor = hls::Point_<int>(seed.x, seed.y - 1);
					if ((seed.y != 0) && (countnum[seed.y - 1][seed.x] == 255))
					{
						countnum[seed.y - 1][seed.x] = labelValue;
					//	pointStack.push(neighbor);
						pointStack2[cnt2].x=neighbor.x;
						pointStack2[cnt2].y=neighbor.y;
						cnt2++;
						area++;
						if (topBoundary > neighbor.y)
							topBoundary = neighbor.y;
					}

					//seed = pointStack.top();
				//	pointStack.pop();
					seed.x = pointStack2[cnt2-1].x;
					seed.y = pointStack2[cnt2-1].y;
					pointStack2[cnt2-1].x=0;
					pointStack2[cnt2-1].y=0;
//					for(int k=0;k<area-1;k++)
//					{
//						pointStack2[k]=pointStack2[k+1];
//						if(k==area-2)
//						{
//							pointStack2[k].x=0;
//						    pointStack2[k].y=0;
//						}
//					}
					cnt2=cnt2-1;
				}

				feather.area = area;
				feather.label = labelValue;
				featherList[cnt].area = feather.area;
				featherList[cnt].label = feather.label;
				cnt++;
				cnt3=cnt;
			}
		}
	}
	for(int k=0;k<MAX_HEIGHT;k++)
		for(int z=0;z<MAX_WIDTH;z++)
	{
		countnum2[k][z]=countnum[k][z];
	}
	return labelValue;
}
/**
 * @description: 根据连通域标签提取手部
 * @param {type} Feather featherList 连通域标签
 * @return: none
 */
void changecolor(Feather featherList[], RGB_IMAGE &src, RGB_IMAGE &dst)
{
	for (int i = 0; i < cnt3; i++)
	{

		// cout << it->label << "\t" << it->area << endl;
	//	cout << featherList[i].label << '\t' << featherList[i].area << endl;
		if (featherList[i].area > areamax)
		{
			labelmax = featherList[i].label;
			areamax = featherList[i].area;
		}
	}

	RGB_PIXEL outPix;
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{

			if (countnum2[i][j] != labelmax)
			{
				outPix.val[0] = 0;
				outPix.val[1] = 0;
				outPix.val[2] = 0;
			}
			else
			{
				outPix.val[0] = 255;
				outPix.val[1] = 255;
				outPix.val[2] = 255;
			}

			dst << outPix;
		}
	}
}
/**
 * @description: 边缘检测
 * @param {type} input 输入图像
 * @return: output输出边缘检测
 */
void hls::hls_trouble(RGB_IMAGE &input, RGB_IMAGE &output, int rows, int cols)
{
	RGB_PIXEL lastinPix;
	double sum_x = 0, sum_y = 0, area = 0;
mirror_ROWS:
	for (int row = 0; row < rows; row++)
	{
		//#pragma HLS loop_flatten off
		lastinPix.val[0] = 0;
		lastinPix.val[1] = 0;
		lastinPix.val[2] = 0;

	mirror_COLS:
		for (int col = 0; col < cols; col++)
		{
#pragma HLS pipeline II = 1 off //--------------------------优化指令1
			RGB_PIXEL curinPix;
			RGB_PIXEL outPix;

			if (row < rows && col < cols)
			{
				input >> curinPix;
			}
			else
			{
			}

			if (curinPix.val[0] == 255 && curinPix.val[1] == 255 && curinPix.val[2] == 255)
			{
				sum_x = sum_x + row; //计算第Ｋ区域的横坐标总和
				sum_y = sum_y + col; //计算第Ｋ区域的纵坐标总和
				area = area + 1;
			}
			if (curinPix.val[0] == 0 && curinPix.val[1] == 0 && curinPix.val[2] == 0 && lastinPix.val[0] == 255 && lastinPix.val[1] == 255 && lastinPix.val[2] == 255)
			{
				outPix.val[0] = 255;
				outPix.val[1] = 255;
				outPix.val[2] = 255;
				//        	 sum_x=sum_x+row;  //计算第Ｋ区域的横坐标总和
				//        	 sum_y=sum_y+col;  //计算第Ｋ区域的纵坐标总和
				//        	 area=area+1;
			}
			else
			{
				if (curinPix.val[0] == 255 && curinPix.val[1] == 255 && curinPix.val[2] == 255 && lastinPix.val[0] == 0 && lastinPix.val[1] == 0 && lastinPix.val[2] == 0)
				{
					outPix.val[0] = 255;
					outPix.val[1] = 255;
					outPix.val[2] = 255;
					//                 sum_x=sum_x+row;  //计算第Ｋ区域的横坐标总和
					//                  sum_y=sum_y+col;  //计算第Ｋ区域的纵坐标总和
					//
					//                  area=area+1;
				}
				else
				{
					outPix.val[0] = 0;
					outPix.val[1] = 0;
					outPix.val[2] = 0;
				}
			}

			lastinPix.val[0] = curinPix.val[0];
			lastinPix.val[1] = curinPix.val[1];
			lastinPix.val[2] = curinPix.val[2];

			output << outPix;
		}
	}
	center.y = round(sum_x / area);
	center.x = round(sum_y / area);
	cout << center.x << '\t' << center.y << endl;
}
/**
 * @description: 肤色的检测
 * @param {type} 肤色检测的月阈值
 * @return: 
 */
void hls::hls_skin_dection(RGB_IMAGE &src, RGB_IMAGE &dst, int rows, int cols,
						   int y_lower, int y_upper, int cb_lower, int cb_upper, int cr_lower, int cr_upper)
{

LOOp_ROWS:
	for (int row = 0; row < rows; row++)
	{
		//#pragma HLS loop_flatten off
	LOOp_COLS:
		for (int col = 0; col < cols; col++)
		{
#pragma HLS pipeline II = 1 off //--------------------------优化指令1
			//变量定义
			RGB_PIXEL src_data;
			RGB_PIXEL pix;
			RGB_PIXEL dst_data;
			bool skin_region;

			if (row < rows && col < cols)
			{
				src >> src_data;
			}

			//获取RGB图像通道数据，注意顺序
			uchar B = src_data.val[0];
			uchar G = src_data.val[1];
			uchar R = src_data.val[2];

			//RGB-->YCbCr颜色空间转换
			uchar y = (76 * R + 150 * G + 29 * B) >> 8;
			uchar cb = ((128 * B - 43 * R - 85 * G) >> 8) + 128;
			uchar cr = ((128 * R - 107 * G - 21 * B) >> 8) + 128;

			//肤色区域判定
			if (y > y_lower && y < y_upper && cb > cb_lower && cb < cb_upper && cr > cr_lower && cr < cr_upper)
				skin_region = 1;
			else
				skin_region = 0;
			//将肤色区域的值大小置为255，即白色
			uchar temp0 = (skin_region == 1) ? (uchar)255 : 0;
			uchar temp1 = (skin_region == 1) ? (uchar)255 : 0;
			uchar temp2 = (skin_region == 1) ? (uchar)255 : 0;

			dst_data.val[0] = temp0;
			dst_data.val[1] = temp1;
			dst_data.val[2] = temp2;
			//复制处理完成后输出图像
			//countnum[row][col] = dst_data.val[0];
			//countnum2[row][col] = dst_data.val[0];
			dst << dst_data;
		}
	}
}

/**
 * @description: 该函数的作用主要是把上面的函数的输入输出端口做一个封装，以便处理视频输入信号，用于将上面的各个检测用代码AXI_STREAM结构封装端口
 * @param {type} AXI_STREAM封装端口i nput
 * @return: 
 */
void ImgProcess_Top(AXI_STREAM &input, AXI_STREAM &output, int rows, int cols,
					int y_lower, int y_upper, int cb_lower, int cb_upper, int cr_lower, int cr_upper)

{
///*
#pragma HLS RESOURCE variable = input core = AXIS metadata = "-bus_bundle INPUT_STREAM"
#pragma HLS RESOURCE variable = output core = AXIS metadata = "-bus_bundle OUTPUT_STREAM"

//#pragma HLS RESOURCE core = AXI_SLAVE variable = rows metadata = "-bus_bundle CONTROL_BUS"
//#pragma HLS RESOURCE core = AXI_SLAVE variable = cols metadata = "-bus_bundle CONTROL_BUS"
//#pragma HLS RESOURCE core = AXI_SLAVE variable = y_lower metadata = "-bus_bundle CONTROL_BUS"
//#pragma HLS RESOURCE core = AXI_SLAVE variable = y_upper metadata = "-bus_bundle CONTROL_BUS"
//#pragma HLS RESOURCE core = AXI_SLAVE variable = cb_lower metadata = "-bus_bundle CONTROL_BUS"
//#pragma HLS RESOURCE core = AXI_SLAVE variable = cb_upper metadata = "-bus_bundle CONTROL_BUS"
//#pragma HLS RESOURCE core = AXI_SLAVE variable = cr_lower metadata = "-bus_bundle CONTROL_BUS"
//#pragma HLS RESOURCE core = AXI_SLAVE variable = cr_upper metadata = "-bus_bundle CONTROL_BUS"
//#pragma HLS RESOURCE core = AXI_SLAVE variable = return metadata = "-bus_bundle CONTROL_BUS"

//#pragma HLS INTERFACE ap_stable port = rows
//#pragma HLS INTERFACE ap_stable port = cols
//#pragma HLS INTERFACE ap_stable port = y_lower
//#pragma HLS INTERFACE ap_stable port = y_upper
//#pragma HLS INTERFACE ap_stable port = cb_lower
//#pragma HLS INTERFACE ap_stable port = cb_upper
//#pragma HLS INTERFACE ap_stable port = cr_lower
//#pragma HLS INTERFACE ap_stable port = cr_upper
#pragma HLS INTERFACE ap_none port=cols
#pragma HLS INTERFACE ap_none port=rows
#pragma HLS INTERFACE ap_none port=y_lower
#pragma HLS INTERFACE ap_none port=y_upper
#pragma HLS INTERFACE ap_none port=cb_lower
#pragma HLS INTERFACE ap_none port=cb_upper
#pragma HLS INTERFACE ap_none port=cr_lower
#pragma HLS INTERFACE ap_none port=cr_upper
#pragma HLS interface ap_ctrl_none port=return
	///*/
	hls::Scalar<3, unsigned char> markcolor(130, 20, 255);
	RGB_IMAGE img_0(rows, cols);
	RGB_IMAGE img_1(rows, cols);
	RGB_IMAGE img_1_1(rows, cols);
	RGB_IMAGE img_1_2(rows, cols);
	RGB_IMAGE img_1_3(rows, cols);
	RGB_IMAGE img_2(rows, cols);
	RGB_IMAGE img_3(rows, cols);
	RGB_IMAGE img_4(rows, cols);
	RGB_IMAGE img_5(rows, cols);
	RGB_IMAGE img_6(rows, cols);
	RGB_IMAGE img_7(rows, cols);
	RGB_IMAGE img_4_1(rows, cols);
	GRAY_IMAGE mask(rows, cols);
	GRAY_IMAGE dmask(rows, cols);
	RGB_IMAGE img_12(rows, cols);
	//Point_<float> (&_keypoints)[100];
	// KT k=20;

	int MAX_KEYPOINTS = 50;
	int line_num = 10;
	int threhold = 90;
	int keypoints[MAX_KEYPOINTS];
	hls::Polar_<double, double> lines[10];
	hls::Point_<int>(_keypoints)[300];

#pragma HLS dataflow
	hls::AXIvideo2Mat(input, img_0); //将video图像数据结构转换为mat型（RGB图像）数据，以便于输入到下面的检测函数中。
	hls::hls_skin_dection(img_0, img_1, rows, cols, y_lower, y_upper, cb_lower, cb_upper, cr_lower, cr_upper);
	//    hls::Sobel<1,0,3>(img_1,img_2);
	Feather featherList[50]; // 存放连通域特征
	int labelValue;
	hls::Duplicate(img_1, img_1_1, img_1_2);
	labelValue = bwLabel(img_1_1, featherList);
	cout << "区域" << labelValue << endl;
	changecolor(featherList, img_1_2, img_1_3);
	hls_trouble(img_1_3, img_2, rows, cols);
	hls::Duplicate(img_2, img_3, img_4);
	hls::FASTX(img_3, _keypoints, threhold, false);
	// vector<hls::Point_<int> > couPoint(_keypoints,_keypoints+300);
	// cout<<couPoint.size()<<endl;
	hls::Duplicate(img_4, img_4_1, img_5);
	hls::FASTX(img_4_1, mask, threhold, false);
	//hls::FASTX(img_3,keypoints, MAX_KEYPOINTS, threhold,true);
	hls::Dilate(mask, dmask);
	PaintMask(img_5, dmask, img_6, markcolor);
	//hls::hls_skin_dection(img_0,img_2,rows,cols,y_lower,y_upper,cb_lower,cb_upper,cr_lower,cr_upper);
//————————————————————————————————原来霍夫检测部分的代码——————————————————————
	//hls::HoughLines2()；
	//    int theshold=60;
	////    hls_trouble(img_1,img_2,rows, cols);
	//    hls::Duplicate (img_2,img_3,img_4);
	//    hls::HoughLines2<4,4>(img_3, lines, theshold);
	//
	//    myPoint line_1[line_num];
	//    myPoint line_2[line_num];
	//    int flag;
	//  //  依次在图中绘制出每条线段
	//        	for( int i = 0; i <line_num ; i++ )
	//        	{
	//        		float rho = lines[i].rho, theta = lines[i].angle;
	//        		//myPoint pt1, pt2;
	//        		double a = cos(theta), b = sin(theta);
	//        		double x0 = a*rho, y0 = b*rho;
	//        		line_1[i].x = round(x0 + theshold*(-b));
	//        		line_1[i].y = round(y0 + theshold*(a));
	//        		line_2[i].x = round(x0 - theshold*(-b));
	//        		line_2[i].y = round(y0 - theshold*(a));
	//        		//RGB_IMAGE img_4(rows, cols);
	//        		//hls::AddS( img_2,img_4);
	//        		cout<<a<<'\t'<<b;
	//        		cout<<'\t'<<line_1[i].x<<'\t'<<line_1[i].y<<'\t'<<line_2[i].x<<'\t'<<line_2[i].y<<endl;
	//        		//RGB_PIXEL data;
	//
	//        		//img_3.write(RGB_PIXEL pt1)
	//
	//        	}
	//
	////    for(  int i = 0; i <line_num ; i++ )
	////   {
	////    	line_x[i] = int(lines[i].rho*cos(lines[i].angle));
	////    	line_y[i] = int(lines[i].rho*sin(lines[i].angle));
	////   }
	//
	//       for(int row = 0; row < rows ; row++)
	//        {
	//
	//           for(int col = 0; col < cols; col++)
	//            {
	//            #pragma HLS pipeline II=1 off
	//
	//             RGB_PIXEL myoutPix;
	//             RGB_PIXEL myinPix;
	//
	//             if(row < rows && col < cols) {
	//                  img_4 >> myinPix;
	//              }
	//              else{
	//              }
	//
	//             flag=0;
	//             for(  int i = 0; i <line_num ; i++ )
	//            {
	//                if(abs(row-line_1[i].x)<2 && abs(col-line_1[i].y)<2){
	//                flag=1;
	//               }
	//                if(abs(row-line_2[i].x)<2 && abs(col-line_2[i].y)<2){
	//                                flag=1;
	//                               }
	//            }
	//
	//             if(flag==1){
	//            	 myoutPix.val[0]=234;
	//            	 myoutPix.val[1]=155;
	//            	 myoutPix.val[2]=255;
	//             }
	//             else{
	//
	//                 myoutPix.val[0]=myinPix.val[0];
	//                 myoutPix.val[1]=myinPix.val[1];
	//                 myoutPix.val[2]=myinPix.val[2];
	//             }
	//
	//              img_5 << myoutPix;
	//
	//            }
	//        }
	//
	////       for(int row = 0; row < rows ; row++)
	////        {
	////
	////           for(int col = 0; col < cols; col++)
	////            {
	////            #pragma HLS pipeline II=1 off
	////
	////             RGB_PIXEL myoutPix2;
	////
	////             myoutPix2.val[0]=(row+col)%255;
	////             myoutPix2.val[1]=(row+col)%255;
	////             myoutPix2.val[2]=(row+col)%255;
	////             img_6<< myoutPix2;
	////
	////            }
	////        }

	
	///////////////----------------- 寻找指尖------------------------//////////////

	hls::Point_<int> tmp;
	hls::Point_<int>(fingerTips)[5];
	int max(0), count(0), notice(0);
	int num = 0;
	for (int i = 0; i < 300; i++)
	{
		tmp = _keypoints[i];
		int dist = (tmp.x - center.x) * (tmp.x - center.x) + (tmp.y - center.y) * (tmp.y - center.y);
		if (dist > max)
		{
			max = dist;
			notice = i;
			//num++;
		}

		// 计算最大值保持的点数，如果大于40（这个值需要设置，本来想根据max值来设置，
		// 但是不成功，不知道为何），那么就认为这个是指尖
		if (dist != max)
		{
			count++;
			if (count > 20)
			{
				count = 0;
				max = 0;
				bool flag = false;
				// 低于手心的点不算
				if (center.y < _keypoints[notice].y)
					continue;
				// 离得太近的不算
				int kk;
				for (kk = 0; kk < 5; kk++)
				{
					if (fingerTips[kk].x == 0)
						break;
				}
				for (int j = 0; j < kk; j++)
				{
					if (abs(_keypoints[notice].x - fingerTips[j].x) < 40)
					{
						flag = true;
						break;
					}
				}

				if (flag)
					continue;
				fingerTips[num] = _keypoints[notice];
				if (abs(center.y - fingerTips[num].y) < 30)
				{
					fingerTips[num].x = 0;
					fingerTips[num].y = 0;
					continue;
				}
				else
				{
					num++;
				}
			}
		}
	}
	//           	for (int j = 0; j < fingerTips.size(); j++){
	//           		finger[j].x=fingerTips[j].x;
	//           		finger[j].y=fingerTips[j].y;
	//           	}

	for (int i = 0; i < num; i++)
	{
		cout << fingerTips[i].x << '\t' << fingerTips[i].y << endl;
	}
	cout << "个数为" << num;

	int myflag = 0;
	for (int row = 0; row < rows; row++)
	{

		for (int col = 0; col < cols; col++)
		{
#pragma HLS pipeline II = 1 off

			RGB_PIXEL myoutPix;
			RGB_PIXEL myinPix;

			if (row < rows && col < cols)
			{
				img_6 >> myinPix;
			}
			else
			{
			}

			myflag = 0;
			for (int i = 0; i < num; i++)
			{
				if (abs(row - fingerTips[i].y) < 3 && abs(col - fingerTips[i].x) < 3)
				{
					myflag = 1;
				}
				if (abs(row - center.y) < 5 && abs(col - center.x) < 5)
				{
					myflag = 1;
				}
			}

			if (myflag == 1)
			{
				myoutPix.val[0] = 255;
				myoutPix.val[1] = 255;
				myoutPix.val[2] = 255;
			}
			else
			{

				myoutPix.val[0] = myinPix.val[0];
				myoutPix.val[1] = myinPix.val[1];
				myoutPix.val[2] = myinPix.val[2];
			}

			img_7 << myoutPix;
		}
	}

	// plot digit
	for (int row = 0; row < rows; row++)
	{

		for (int col = 0; col < cols; col++)
		{
#pragma HLS pipeline II = 1 off

			RGB_PIXEL myoutPix;
			RGB_PIXEL myinPix;

			if (row < rows && col < cols)
			{
				img_7 >> myinPix;
			}
			else
			{
			}
			myflag = 0;
			if (num >= 5)
			{
				if (row == 10 && col > 250 && col < 270)
				{
					myflag = 1;
				}
				if (col == 250 && row > 10 && row < 40)
				{
					myflag = 1;
				}
				if (row == 40 && col > 250 && col < 270)
				{
					myflag = 1;
				}
				if (col == 270 && row > 40 && row < 70)
				{
					myflag = 1;
				}
				if (row == 70 && col > 250 && col < 270)
				{
					myflag = 1;
				}
			}

			if (num == 4)
			{
				if (col == 270 && row > 10 && row < 40)
				{
					myflag = 1;
				}
				if (col == 250 && row > 10 && row < 40)
				{
					myflag = 1;
				}
				if (row == 40 && col > 250 && col < 270)
				{
					myflag = 1;
				}
				if (col == 270 && row > 40 && row < 70)
				{
					myflag = 1;
				}
			}

			if (num == 3)
			{
				if (row == 10 && col > 250 && col < 270)
				{
					myflag = 1;
				}
				if (col == 270 && row > 10 && row < 40)
				{
					myflag = 1;
				}
				if (row == 40 && col > 250 && col < 270)
				{
					myflag = 1;
				}
				if (col == 270 && row > 40 && row < 70)
				{
					myflag = 1;
				}
				if (row == 70 && col > 250 && col < 270)
				{
					myflag = 1;
				}
			}

			if (num == 2)
			{
				if (row == 10 && col > 250 && col < 270)
				{
					myflag = 1;
				}
				if (col == 270 && row > 10 && row < 40)
				{
					myflag = 1;
				}
				if (row == 40 && col > 250 && col < 270)
				{
					myflag = 1;
				}
				if (col == 250 && row > 40 && row < 70)
				{
					myflag = 1;
				}
				if (row == 70 && col > 250 && col < 270)
				{
					myflag = 1;
				}
			}

			if (num == 1)
			{

				if (col == 270 && row > 10 && row < 70)
				{
					myflag = 1;
				}
			}
			if (myflag == 1)
			{
				myoutPix.val[0] = 0;
				myoutPix.val[1] = 215;
				myoutPix.val[2] = 255;
			}
			else
			{

				myoutPix.val[0] = myinPix.val[0];
				myoutPix.val[1] = myinPix.val[1];
				myoutPix.val[2] = myinPix.val[2];
			}

			img_12 << myoutPix;
		}
	}
	hls::Mat2AXIvideo(img_12, output); //将mat型（RGB图像）数据转换为video图像数据结构输出
}
