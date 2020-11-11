#include "xf_crop_resize_config.h"

void crop_resize_accel(xf::cv::Mat<TYPE, HEIGHT, WIDTH, NPC>& _src,
					   xf::cv::Mat<TYPE, NEWHEIGHT, NEWWIDTH, NPC>& _crp,
                       xf::cv::Mat<TYPE, NEWHEIGHT, NEWWIDTH, NPC>& _dst,
                       int roi[4])
{
    xf::cv::Rect_ <unsigned int> roi_p[1];
    roi_p[0].x =roi[0];
    roi_p[0].y =roi[1];
    roi_p[0].height=roi[2];
    roi_p[0].width=roi[3];
    
    xf::cv::Mat<XF_8UC3,HEIGHT,WIDTH,NPC> cout_mat(roi_p[0].height,roi_p[0].width);
    
    //xf::cv::crop<XF_8UC3,HEIGHT,WIDTH,STREAM,NPC>(_src,_dst,roi_p[0]);

    xf::cv::crop<XF_8UC3,HEIGHT,WIDTH,STREAM,NPC>(_src,_crp,roi_p[0]);
    xf::cv::resize<INTERPOLATION, XF_8UC3, HEIGHT, WIDTH, NEWHEIGHT, NEWWIDTH, NPC, MAXDOWNSCALE>(_crp,_dst);
    
    
}
