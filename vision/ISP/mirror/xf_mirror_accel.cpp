#include "xf_mirror_config.h"

// reading in the color image
void xf_mirror_accel(xf::cv::Mat<XF_8UC3,HEIGHT,WIDTH,NPC> &rgb_img,
        	                     xf::cv::Mat<XF_8UC3,HEIGHT,WIDTH,NPC> &rgb_img_out,
       	                     int mirror_type)
	{
#pragma HLS STREAM variable=rgb_img_out dim=1
#pragma HLS STREAM variable=rgb_img dim=1
#pragma HLS DATAFLOW
		int img_rows = HEIGHT;
		int img_cols = WIDTH;


		//int img_rows_2 = rgb_img.rows / 2;
		//int img_cols_2 = rgb_img.cols / 2;

        xf::cv::Mat<XF_32FC1,HEIGHT,WIDTH,NPC> X;
#pragma HLS STREAM variable=X.data depth=2 dim=1
        xf::cv::Mat<XF_32FC1,HEIGHT,WIDTH,NPC> Y;
#pragma HLS STREAM variable=Y.data depth=2 dim=1


		//xf::cv::Mat X(img_rows, img_cols, CV_32F, 0.00);
		//xf::cv::Mat Y(img_rows, img_cols, CV_32F, 0.00);
        /*
		if (mirror_type == 1)
		{
			up_to_down1:
			for (int i = 0; i < img_rows; i++)//上下
			{
#pragma HLS DATAFLOW
				up_to_donw2:
				for (int j = 0; j < img_cols; j++)
				{
#pragma HLS DATAFLOW
					Y.write_float(i*WIDTH+j, (float)(img_rows - i));
					X.write_float(i*WIDTH+j, (float)j);				
				}
			}
		}
		else if (mirror_type == 2)
		{*/
            right_to_left1:
			for (int i = 0; i < img_rows; i++)//左右
			{
				right_to_left2:
				for (int j = 0; j < img_cols; j++)
				{
					Y.write_float(i*WIDTH+j, (float)i);
					X.write_float(i*WIDTH+j, (float)(img_cols - j));
				}
			}
		/*}
		else
		{
			ur_to_dl1:
			for (int i = 0; i < img_rows; i++)//上下左右
			{
#pragma HLS DATAFLOW
				ur_to_dl2:
				for (int j = 0; j < img_cols; j++)
				{
#pragma HLS DATAFLOW
					Y.write_float(i*WIDTH+j, (float)(img_rows - i));
					X.write_float(i*WIDTH+j, (float)(img_cols - j));
				}
			}
		}
*/


/*
        if (mirror_type == 1)
		{
			for (int i = 0; i < img_rows; i++)//上下
			{
				for (int j = 0; j < img_cols; j++)
				{
					Y.data[i*WIDTH+j] = img_rows - i;
					X.data[i*WIDTH+j] = j;
				}
			}
		}
		else if (mirror_type == 2)
		{
			for (int i = 0; i < img_rows; i++)//左右
			{
				for (int j = 0; j < img_cols; j++)
				{
					Y.data[i*WIDTH+j] = i;
					X.data[i*WIDTH+j] = img_cols - j;
				}
			}
		}
		else
		{
			for (int i = 0; i < img_rows; i++)//上下左右
			{
				for (int j = 0; j < img_cols; j++)
				{
					Y.data[i*WIDTH+j] = img_rows - i;
					X.data[i*WIDTH+j] = img_cols - j;
				}
			}
		}
*/


xf::cv::remap<XF_WIN_ROWS, XF_REMAP_INTERPOLATION, TYPE, XF_32FC1, TYPE, XF_HEIGHT, XF_WIDTH, XF_NPPC1,XF_USE_URAM>(rgb_img, rgb_img_out, X, Y);
}


    
