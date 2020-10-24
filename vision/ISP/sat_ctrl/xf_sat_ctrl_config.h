#ifndef _XF_SAT_CTRL_CONFIG_
#define _XF_SAT_CTRL_CONFIG_

#include <hls_stream.h>
#include <hls_math.h>
#include <ap_int.h>
#include<cassert>
//#include<vector>
//using namespace cv;
//using namespace std;

#include "common/xf_common.hpp"
#include "core/xf_convert_bitdepth.hpp"
#include "common/xf_utility.hpp"
#include "core/xf_magnitude.hpp"
#include "core/xf_mean_stddev.hpp"

#define PRAGMA_SUB(x) _Pragma(#x)
#define DYN_PRAGMA(x) PRAGMA_SUB(x)

#define NPC XF_NPPC1
#define INTYPE XF_8UC3

#define WIDTH 1920
#define HEIGHT 1080

void sat_ctrl_accel(xf::cv::Mat<XF_8UC1, HEIGHT, WIDTH, NPC>& U,
                    xf::cv::Mat<XF_8UC1, HEIGHT, WIDTH, NPC>& V,
                    xf::cv::Mat<XF_8UC2, HEIGHT, WIDTH, NPC>& UV,
                    unsigned char mode,
                    int sat);


#endif
