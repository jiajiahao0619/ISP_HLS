/*
 * Copyright 2019 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "common/xf_headers.hpp"
#include "xf_gammacorrection_config.h"
#include<cmath>
#include<math.h>
#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include<vector>
using namespace std;

//#include <CL/cl.h>
//#include "xcl2.hpp"
cv::Mat GC(cv::Mat img, float gamma = 1.3853, int clip = 255)
{
	cv::Mat yuv_img;
	cv::Mat out_yuv;
	cv::Mat out_bgr;
	cv::Mat color_corrected[3];
	vector<cv::Mat> channels;
	//cvtColor(img, yuv_img, CV_RGB2YUV);
	//cv::split(yuv_img, color_corrected);
	cv::split(img, channels);
	cv::Mat Y = channels.at(0);
	cv::Mat Y_32F;
	Y.convertTo(Y_32F, CV_32F);
	uchar T[256];
	for (int i = 0; i < Y_32F.rows; i++)
	{
		for (int j = 0; j < Y_32F.cols; j++)
		{
			Y.at<uchar>(i, j) = pow((Y_32F.at<float>(i, j)) / clip, gamma)*clip;
		}
	}
	color_corrected[0] = Y;
	channels.at(0)=Y;
	cv::merge(channels, out_yuv);
	//cvtColor(out_yuv, out_bgr, CV_YUV2RGB);
	cv::imwrite("out_GC_rgb_me.jpg", out_yuv);
	return out_yuv;
}

float mean_pixel(cv::Mat img) {
    if (img.channels() > 2) {
        cvtColor(img.clone(), img, CV_BGR2GRAY);
        return cv::mean(img)[0];
    } else {
        return cv::mean(img)[0];
    }
}
float auto_gamma_value(cv::Mat img) {
    float max_pixel = 255;
    float middle_pixel = 128;
    float pixel_range = 256;
    float mean_l = mean_pixel(img);

    float gamma = log(middle_pixel / pixel_range) / log(mean_l / pixel_range); // Formula from ImageJ

    return gamma;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Invalid Number of Arguments!\nUsage:\n");
        fprintf(stderr, "<Executable Name> <input image path1> \n");
        return -1;
    }

    cv::Mat in_gray, ocv_ref, out_gray, diff;

    in_gray.create(cv::Size(WIDTH,HEIGHT),CV_8UC3);

    in_gray = cv::imread(argv[1], 0); // read image
    if (in_gray.data == NULL) {
        fprintf(stderr, "Cannot open image %s\n", argv[1]);
        return -1;
    }
    ocv_ref.create(in_gray.rows, in_gray.cols, CV_8UC3);
    out_gray.create(in_gray.rows, in_gray.cols, CV_8UC3);
    diff.create(in_gray.rows, in_gray.cols, CV_8UC3);
    float ref_gamma = 2.54567;
    ocv_ref=GC(in_gray,ref_gamma,255);


    float gamma_ = auto_gamma_value(in_gray);

    static xf::cv::Mat<IN_TYPE, HEIGHT, WIDTH, NPC1> imgInput1(in_gray.rows, in_gray.cols);
    static xf::cv::Mat<OUT_TYPE, HEIGHT, WIDTH, NPC1> imgOutput(out_gray.rows, out_gray.cols);

    imgInput1.copyTo(in_gray.data);

    xf::cv::imwrite("in_hls.jpg", imgInput1);
    printf("gamma=%f\n",gamma_);
    gammacorrection_accel(imgInput1, imgOutput, ref_gamma);

    xf::cv::imwrite("out_hls.jpg", imgOutput);
    //compare
    xf::cv::absDiff(ocv_ref,imgOutput,diff);
    float err_per;
    xf::cv::analyzeDiff(diff,0,err_per);
    cv::imwrite("diff_img.jpg",diff);

    in_gray.~Mat();
    ocv_ref.~Mat();
    out_gray.~Mat();
    diff.~Mat();

	if(err_per > 0.0f)
	{
	    printf("DIFF error");
		return 1;
	}
	printf("PASS\n");
	return 0;

}
