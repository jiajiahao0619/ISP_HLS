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

//#ifndef _XF_SAT_CONFIG_
//#define _XF_SAT_CONFIG_

#include <hls_stream.h>
#include <hls_math.h>
#include <ap_int.h>

#include "common/xf_common.hpp"
#include "imgproc/xf_resize.hpp"
#include "common/xf_utility.hpp"
#include "imgproc/xf_add_weighted.hpp"
#include "imgproc/xf_threshold.hpp"
#include "imgproc/xf_channel_combine.hpp"



typedef ap_uint<8>      ap_uint8_t;
typedef ap_uint<64>      ap_uint64_t;



#define NPC XF_NPPC1
#define NPIX XF_NPPC1

#define IN_TYPE  XF_8UC1
#define OUT_TYPE XF_8UC2


// port widths

// For Nearest Neighbor & Bilinear Interpolation, max down scale factor 2 for all 1-pixel modes, and for upscale in x
// direction

/* Interpolation type*/
#define INTERPOLATION 1
// 0 - Nearest Neighbor Interpolation
// 1 - Bilinear Interpolation
// 2 - AREA Interpolation

/* Input image Dimensions */
#define WIDTH 128  // Maximum Input image width
#define HEIGHT 128 // Maximum Input image height

#define THRESH_TRUNC_T 2
#define THRESH_TOZERO_T 3




//#if NO
//#define NPC1 XF_NPPC1
//#endif
//#if RO
#define NPC1 XF_NPPC8
//#endif

/*
#if GRAY
#define IN_TYPE XF_8UC1
#define OUT_TYPE XF_8UC1
#else
#define IN_TYPE XF_8UC3
#define OUT_TYPE XF_8UC3
*/


/* Interface types*/


extern "C" void xf_sat_accel(
		xf::cv::Mat<XF_8UC1,HEIGHT,WIDTH,NPC> Y,
		xf::cv::Mat<XF_8UC1,HEIGHT,WIDTH,NPC> U,
        xf::cv::Mat<XF_8UC1,HEIGHT,WIDTH,NPC> V,
        xf::cv::Mat<XF_8UC3,HEIGHT,WIDTH,NPC> &out_U_V,
        float saturation,
        int img_clip);






