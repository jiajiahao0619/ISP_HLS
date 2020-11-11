#ifndef _XF_RGB_SAT_CONFIG_
#define _XF_RGB_SAT_CONFIG_

#include <hls_stream.h>
#include <hls_math.h>
#include <ap_int.h>
#include<cassert>
//#include<vector>
//using namespace cv;
//using namespace std;

#include "common/xf_common.hpp"
#include "common/xf_utility.hpp"
#include "imgproc/xf_add_weighted.hpp"

#define PRAGMA_SUB(x) _Pragma(#x)
#define DYN_PRAGMA(x) PRAGMA_SUB(x)

#define NPC XF_NPPC1
#define INTYPE XF_8UC3

#define WIDTH 1920
#define HEIGHT 1080

#define NEWWIDTH 1920
#define NEWHEIGHT 1080

void rgb_sat_accel(xf::cv::Mat<INTYPE, HEIGHT, WIDTH, NPC>& rgb_in,
                        xf::cv::Mat<INTYPE, HEIGHT, WIDTH, NPC>& rgb_out,
                        float r_sat,
                        float g_sat,
                        float b_sat
                        );


#endif
