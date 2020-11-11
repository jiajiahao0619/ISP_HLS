#include "xf_sat_config.h"
#include "common/xf_headers.hpp"


#include<iostream>
#include<cmath>
#include<math.h>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;

cv::Mat sat(cv::Mat Y,cv::Mat U, cv::Mat V, float saturation = 1.5, int img_clip = 255)
{
	int img_rows = U.rows;
	int img_cols = U.cols;

	cv::Mat img_128(img_rows, img_cols, CV_8UC1, 128);

	addWeighted(U, saturation, img_128, 1 - saturation, 0, U);
	addWeighted(V, saturation, img_128, 1 - saturation, 0, V);
	//addWeighted(Y, saturation, img_128, 1 - saturation, 0, Y);


	threshold(U, U, img_clip, img_clip, THRESH_TRUNC);
	threshold(U, U, 0, img_clip, THRESH_TOZERO);

	threshold(V, V, img_clip, img_clip, THRESH_TRUNC);
	threshold(V, V, 0, img_clip, THRESH_TOZERO);

	//threshold(Y, Y, img_clip, img_clip, THRESH_TRUNC);
	//threshold(Y, Y, 0, img_clip, THRESH_TOZERO);
	cv::Mat out_U_V;
	cv::Mat color_U_V[3];
	color_U_V[0] = V;
	color_U_V[1] = U;
	color_U_V[2] = Y;


	cv::merge(color_U_V, 3, out_U_V);
	return out_U_V;
}


int main(int argc, char** argv){
    if (argc != 2) {
        printf("Usage : <executable> <input image> \n");
        return -1;
    }
    cv::Mat img = cv::imread(argv[1], 1);
	//cv::Mat img = imread("c:/128x128.png");
	if (img.empty())
	{
		printf("%s\n", "����ͼƬʧ�ܣ�");
		system("pause");
		return 0;
	}
	cv::Mat yuv_img;
	cv::Mat diff;
	cv::cvtColor(img, yuv_img, CV_RGB2YUV);
	cv::Mat color_corrected[3];
	cv::split(yuv_img, color_corrected);
	cv::Mat Y = color_corrected[0];
	cv::Mat U = color_corrected[1];
	cv::Mat V = color_corrected[2];



	int clip = 255;
	// <1 saturation down
	// >1 saturation up
	float saturation = 1.0;
	cv::Mat out_rgb;
	cv::Mat out_U_V,out_U_V2;
	cv::Mat color_U_V[3];

	diff.create(cv::Size(WIDTH,HEIGHT),CV_8UC3);
	//out_U_V.create(cv::Size(WIDTH,HEIGHT),CV_8UC3);



    xf::cv::Mat<XF_8UC1, HEIGHT, WIDTH, NPC> out_U_X(HEIGHT, WIDTH);
    xf::cv::Mat<XF_8UC1, HEIGHT, WIDTH, NPC> out_V_X(HEIGHT, WIDTH);
    xf::cv::Mat<XF_8UC1, HEIGHT, WIDTH, NPC> out_Y_X(HEIGHT, WIDTH);
	xf::cv::Mat<XF_8UC3, HEIGHT, WIDTH, NPC> out_U_V_X(HEIGHT, WIDTH);
	//static xf::cv::Mat<XF_8UC3, HEIGHT, WIDTH, NPC> out_U_V_Y(HEIGHT, WIDTH);


    out_U_X.copyTo(U.data);
    out_V_X.copyTo(V.data);
    out_Y_X.copyTo(Y.data);
	out_U_V = sat(Y,U, V, saturation, clip);
    xf_sat_accel(out_Y_X,out_U_X,out_V_X,out_U_V_X,saturation,clip);

    float err_per;


    xf::cv::absDiff(out_U_V,out_U_V_X, diff);


    //xf::cv::absDiff(out_U_V,out_U_V_Y, diff);


    xf::cv::analyzeDiff(diff, 0, err_per);


    if(err_per > 0.0f)
    {
        printf("DIFF error");
        return 1;
    }




	cv::split(out_U_V, color_U_V);
	color_corrected[1] = color_U_V[0];
	color_corrected[2] = color_U_V[1];
	color_corrected[0] = color_U_V[2];


	merge(color_corrected, 3, yuv_img);


	cvtColor(yuv_img, out_rgb, CV_YUV2RGB);

	//imshow("修改后的图像_自动", out_contrast_auto);
	//imshow("修改后的图像_手动", out_contrast_manual);
	cv::imwrite("results_sat.jpg", out_rgb);

	return 0;
}
