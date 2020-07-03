#include "top.h"
#include "hls_opencv.h"
#include "iostream"
#include <time.h>

using namespace std;
using namespace cv;
//vector<hls::Point_<int> > fingerTips;
int main (int argc, char** argv)
{
    IplImage* src = cvLoadImage(INPUT_IMAGE);
    //IplImage* src = cvLoadImage("test.jpg");
    //IplImage* src = cvLoadImage("test_img1.jpg");
  //  vector<hls::Point_<int> > fingerTips;

    IplImage* dst = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    AXI_STREAM  src_axi, dst_axi;
    IplImage2AXIvideo(src, src_axi);
    hls::Polar_<float,float> lines[10];
    vector<Point> data;
    Mat image=imread("1pic.jpg");
    Mat drawing;
//	drawing =  Mat::zeros(image.size(), CV_8UC3);
//	image.copyTo(drawing);
    Point center(146,208);
    ImgProcess_Top(src_axi, dst_axi, src->height, src->width,1,255,75,125,133,190);
//    for (int j = 0; j < fingerTips.size(); j++)
//    {
//        data[j].x=fingerTips[j].x;
//        data[j].y=fingerTips[j].y;
//        cout<<data[j].x<<data[j].y<<endl;
//    	circle(image, data[j], 6, Scalar(0, 255, 0), -1);
//         line(image, center, data[j], Scalar(255, 0, 0), 2);
//    }
   // circle(image, center, 6, Scalar(0, 255, 0), -1);
   // imshow("image",image);
   // imwrite("result.jpg", image);
  //  waitKey();
    AXIvideo2IplImage(dst_axi, dst);

//    cvShowImage("src",src);
//    cvShowImage("dst_hls",dst);
//    waitKey(0);//参数<=0时等待按键事件发生，按下键的话返回按键的值， 否则返回-1；
    cvSaveImage(OUTPUT_IMAGE, dst);
    cvReleaseImage(&src);
    cvReleaseImage(&dst);

    return 0;
}
