/*
 * @Date: 2020-04-27 16:14:20
 * @LastEditors: LHD
 * @LastEditTime: 2020-04-27 16:48:25
 * @FilePath: \gesture_detect\skin_clr.cpp
 * @Description: 鑲よ壊锟�?锟�?
 */
#include "top.h"
#include <string.h>
 

void skin_dection(RGB_IMAGE& src, RGB_IMAGE& dst){

    RGB_IMAGE binImage, tmp, tmp1;
    RGB_IMAGE  Y, Cr, Cb, H;
    RGB_IMAGE channels[3], channels1[3];

    //hls::CvtColor(src,);
    // 棰滆壊绌洪棿鍙樻崲锛圧GB to GRAY锟�?
         hls::CvtColor<HLS_BGR2GRAY>(src, binImage);

		// 棰滆壊绌洪棿鍙樻崲锛圧GB to YCrCb锟�?

		hls::CvtColor<HLS_BGR2GRAY>(src, tmp);

		// 棰滆壊绌洪棿鍙樻崲锛圧GB to HSV锟�?

		hls::CvtColor<HLS_BGR2GRAY>(src, tmp1);
        // 閫氶亾鍒嗙
		hls::Split(tmp, channels[0],channels[1],channels[2]);
		hls::Split(tmp1, channels1[0],channels1[1],channels1[2]);
		Cr = channels[1]; // 鍒嗙鍑猴拷?锟借壊璋僀r锟�?
		Cb = channels[2]; // 鍒嗙鍑猴拷?锟介ケ鍜屽害Cb锟�?
		H = channels1[0]; // 鍒嗙鍑猴拷?锟紿锟�?

		// 鑲よ壊锟�?娴嬶紝杈撳嚭浜岋拷?锟藉浘锟�?
		for (int j = 1; j < Cr.rows - 1; j++) // 閬嶅巻鍥惧儚鍍忕礌锟�?
		{
			for (int i = 1; i < Cb.cols - 1; i++)
			{
			uchar currentCr = Cr.at<uchar>(i,j);
			uchar currentCb = Cb[i][j];
			uchar currentH = H[i][j];
			//uchar *current = binImage<uchar>(j);


				if ((currentCr >= 135) && (currentCr <= 170) && (currentCb >= 94) && (currentCb <= 125) && (currentH >= 1) && (currentH <= 23))
					binImage[i][j] = 255;
				else
					binImage[i][j] = 0;
			}
		}

		hls::Erode(binImage, binImage);
		hls::Dilate(binImage, dst);

}
void ImgProcess(AXI_STREAM& input, AXI_STREAM& output,int rows, int cols){
	RGB_IMAGE img_0(rows, cols);
	    RGB_IMAGE img_1(rows, cols);
	    #pragma HLS dataflow
	    hls::AXIvideo2Mat(input,img_0);//灏唙ideo鍥惧儚鏁版嵁缁撴瀯杞崲涓簃at鍨嬶紙RGB鍥惧儚锛夋暟鎹紝浠ヤ究浜庤緭鍏ュ埌涓嬮潰鐨勬娴嬪嚱鏁颁腑銆?
	    skin_dection(img_0,img_1);
	    hls::Mat2AXIvideo(img_1, output);//灏唌at鍨嬶紙RGB鍥惧儚锛夋暟鎹浆鎹负video鍥惧儚鏁版嵁缁撴瀯杈撳嚭

}
