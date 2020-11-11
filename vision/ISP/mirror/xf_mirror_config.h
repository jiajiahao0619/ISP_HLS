#ifndef _XF_RGB_GAIN_CONFIG_
#define _XF_RGB_GAIN_CONFIG_

#include <hls_stream.h>
#include <hls_math.h>
#include <ap_int.h>
#include <cassert>
//#include<vector>
//using namespace cv;
//using namespace std;

#include "common/xf_common.hpp"
#include "common/xf_utility.hpp"
#include "imgproc/xf_remap.hpp"
#include "imgproc/xf_remap.hpp"


#define XF_WIN_ROWS 2
//#define XF_WIN_ROWS 8

#define  GRAY 0
#define  RGB 1

#define  DEBUG 1

// The type of interpolation, define "XF_REMAP_INTERPOLATION" as either "XF_INTERPOLATION_NN" or "XF_INTERPOLATION_BILINEAR"
#define XF_REMAP_INTERPOLATION XF_INTERPOLATION_BILINEAR

#define XF_USE_URAM false

#define PRAGMA_SUB(x) _Pragma(#x)
#define DYN_PRAGMA(x) PRAGMA_SUB(x)

#define NPC XF_NPPC1
#define TYPE XF_8UC3
#define INTYPE XF_8UC3


#define XF_WIDTH 1920
#define XF_HEIGHT 1080

#define WIDTH 1920
#define HEIGHT 1080

void xf_mirror_accel(xf::cv::Mat<XF_8UC3,HEIGHT,WIDTH,NPC> &rgb_img,
	                 xf::cv::Mat<XF_8UC3,HEIGHT,WIDTH,NPC> &rgb_img_out,
	                 int mirror_type);


#endif
