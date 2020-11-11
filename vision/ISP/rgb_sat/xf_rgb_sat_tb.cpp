#include "common/xf_headers.hpp"
#include "xf_rgb_sat_config.h"
using namespace std;

cv::Mat Sat(cv::Mat img, int r_sat,int g_sat,int b_sat)//sat 0~100¿Éµ÷
{
	cv::Mat temp[3];
	cv::split(img, temp);
	cv::Mat r = temp[0];
	cv::Mat g = temp[1];
	cv::Mat b = temp[2];

	cv::Mat b_out;
	addWeighted(b, 1.0, b, 0.0, (b_sat - 50)*0.76, b_out);
	cv::Mat g_out;
	addWeighted(g, 1.0, g, 0.0, (g_sat - 50)*1.75, g_out);
	cv::Mat r_out;
	addWeighted(r, 1.0, r, 0.0, (r_sat - 50)*1.25, r_out);

	cv::Mat output;
	cv::Mat color[3];
	color[0] = r_out;
	color[1] = g_out;
	color[2] = b_out;
	merge(color, 3, output);
	return output;
}

int main(int argc, char** argv) {
    cv::Mat img, result_ocv, error;

    if (argc != 2) {
        printf("Usage : <executable> <input image> \n");
        return -1;
    }

#if GRAY
    img.create(cv::Size(WIDTH, HEIGHT), CV_8UC1);
    result_ocv.create(cv::Size(NEWWIDTH, NEWHEIGHT), CV_8UC1);
    error.create(cv::Size(NEWWIDTH, NEWHEIGHT), CV_8UC1);
#else
    img.create(cv::Size(WIDTH, HEIGHT), CV_8UC3);
    result_ocv.create(cv::Size(NEWWIDTH, NEWHEIGHT), CV_8UC3);
    error.create(cv::Size(NEWWIDTH, NEWHEIGHT), CV_8UC3);
#endif

#if GRAY
    // reading in the color image
    img = cv::imread(argv[1], 0);
#else
    img = cv::imread(argv[1], 1);
#endif

    if (!img.data) {
        return -1;
    }
    unsigned short in_width, in_height;
    in_width = img.cols;
    in_height = img.rows;

    result_ocv = Sat(img, 10.0,20.0,70.0);

    cv::imwrite("input.png", img);
    static xf::cv::Mat<INTYPE, HEIGHT, WIDTH, NPC> imgInput(in_height, in_width);
    static xf::cv::Mat<INTYPE, NEWHEIGHT, NEWWIDTH, NPC> imgOutput(in_height, in_width);

    imgInput.copyTo(img.data);
    rgb_sat_accel(imgInput,imgOutput,10.0,20.0,70.0);
    float err_per;
    xf::cv::absDiff(result_ocv, imgOutput, error);
    xf::cv::analyzeDiff(error, 1, err_per);
    xf::cv::imwrite("hls_out.png", imgOutput);
    cv::imwrite("resize_ocv.png", result_ocv);
    cv::imwrite("error.png", error);
    if (err_per > 0.0f) {
        printf("\nTest Failed\n");
        return -1;
    }

    return 0;



}
