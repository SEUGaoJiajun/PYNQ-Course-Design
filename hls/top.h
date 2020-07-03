/*
 * @Date: 2020-04-26 09:26:56
 * @LastEditors: LHD
 * @LastEditTime: 2020-06-19 20:53:28
 * @FilePath: \gesture_detect\top.h
 * @Description: 
 */ 
#ifndef _TOP_H_
#define _TOP_H_

#include "hls_video.h"
#include"hls\hls_video_fast.h"
#include<vector>
using namespace std;
// maximum image size
//#define MAX_WIDTH  435
//#define MAX_HEIGHT 415

#define MAX_WIDTH  300
#define MAX_HEIGHT 400
#define pi      3.14
typedef unsigned char uchar;

// I/O Image Settings
//#define INPUT_IMAGE           "435415pic.jpg"
//#define OUTPUT_IMAGE          "result_435415pic.jpg"

#define INPUT_IMAGE           "7pic.jpg"
#define OUTPUT_IMAGE          "result_282250pic.jpg"
//#define OUTPUT_IMAGE_GOLDEN   "test_img1.jpg"

// typedef video library core structures
typedef hls::stream<ap_axiu<24,1,1,1> >               AXI_STREAM;
typedef hls::Mat<MAX_HEIGHT, MAX_WIDTH, HLS_8UC3>     RGB_IMAGE;
typedef hls::Mat<MAX_HEIGHT, MAX_WIDTH, HLS_8UC1>     GRAY_IMAGE;
typedef hls::Scalar<3, unsigned char>                 RGB_PIXEL;
//extern  vector<hls::Point_<int> > fingerTips;

struct myPoint
{
	int x;
	int y;
};
typedef struct _Feather
{
    int label;        // 连通域的label值
    int area;         // 连通域的面积
} Feather;
//函数声明，，，用到了命名空间，这就是一个虚拟文件夹的意思
namespace hls
{
    void hls_skin_dection(RGB_IMAGE& src, RGB_IMAGE& dst,int rows, int cols,
                          int y_lower,int y_upper,int cb_lower,int cb_upper,int cr_lower,int cr_upper);
    void hls_trouble(RGB_IMAGE& input, RGB_IMAGE& output,int rows, int cols);

}

void ImgProcess_Top(AXI_STREAM& input, AXI_STREAM& output,int rows, int cols,
        int y_lower,int y_upper,int cb_lower,int cb_upper,int cr_lower,int cr_upper);

#endif
