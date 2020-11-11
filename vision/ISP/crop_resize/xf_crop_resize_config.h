#ifndef _XF_CROP_RESIZE_CONFIG_H
#define _XF_CROP_RESIZE_CONFIG_H

#include <hls_stream.h>
//#include <hls_math.h>
#include <ap_int.h>

#include "common/xf_common.hpp"
#include "common/xf_utility.hpp"
#include "imgproc/xf_resize.hpp"
#include "imgproc/xf_crop.hpp"

#define NPC XF_NPPC1
#define TYPE XF_8UC3

#define WIDTH 1920
#define HEIGHT 1080

#define NEWWIDTH 1920
#define NEWHEIGHT 1080

#define RGB 1
#define NUM_ROI 1

#define STREAM 0
//resize
#define INTERPOLATION 1
#define MAXDOWNSCALE 2


void crop_resize_accel(xf::cv::Mat<TYPE, HEIGHT, WIDTH, NPC>& _src,
					   xf::cv::Mat<TYPE, NEWHEIGHT, NEWWIDTH, NPC>& _crp,
                       xf::cv::Mat<TYPE, NEWHEIGHT, NEWWIDTH, NPC>& _dst,
                       int roi[4]);




#endif
