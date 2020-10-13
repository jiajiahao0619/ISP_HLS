#ifndef _XF_RGB_GAIN_CONFIG_
#define _XF_RGB_GAIN_CONFIG_

#include <hls_stream.h>
#include <hls_math.h>
#include <ap_int.h>
#include<cassert>
//#include<vector>
//using namespace cv;
//using namespace std;

#include "common/xf_common.hpp"
#include "common/xf_utility.hpp"

#define PRAGMA_SUB(x) _Pragma(#x)
#define DYN_PRAGMA(x) PRAGMA_SUB(x)

#define NPC XF_NPPC1
#define INTYPE XF_8UC3

#define WIDTH 1920
#define HEIGHT 1080

extern "C" void rgb_gain_accel(xf::cv::Mat<XF_8UC3,HEIGHT,WIDTH,NPC> &rgb_img,
        xf::cv::Mat<XF_8UC3,HEIGHT,WIDTH,NPC> &rgb_img_gain,
        float r_gain,
        float g_gain,
        float b_gain);


#endif
