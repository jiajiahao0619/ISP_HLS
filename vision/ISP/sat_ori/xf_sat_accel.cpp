#include "xf_sat_config.h"



extern "C"{

	void xf_sat_accel(
			          xf::cv::Mat<XF_8UC1,HEIGHT,WIDTH,NPC> Y,
					  xf::cv::Mat<XF_8UC1,HEIGHT,WIDTH,NPC> U,
		              xf::cv::Mat<XF_8UC1,HEIGHT,WIDTH,NPC> V,
		              xf::cv::Mat<XF_8UC3,HEIGHT,WIDTH,NPC> &out_U_V,
		              float saturation, 
		              int img_clip)
	{
		int img_rows = U.rows;
		int img_cols = U.cols;

		xf::cv::Mat<XF_8UC1, HEIGHT, WIDTH, NPC> img_128(HEIGHT, WIDTH);

		 for (int i = 0; i < img_rows; i++)
		 {
			 for (int j = 0; j < img_cols; j++)
			 {
				img_128.data[i*img_cols+j] = 128;
			 }
		  }


		/*void add_weighted_accel(xf::cv::Mat<IN_TYPE, HEIGHT, WIDTH, NPC1>& imgInput1,
                        float alpha,
                        xf::cv::Mat<IN_TYPE, HEIGHT, WIDTH, NPC1>& imgInput2,
                        float beta,
                        float gama,
                        xf::cv::Mat<OUT_TYPE, HEIGHT, WIDTH, NPC1>& imgOutput);*/



		xf::cv::addWeighted(U, saturation, img_128, 1 - saturation, 0, U);
		xf::cv::addWeighted(V, saturation, img_128, 1 - saturation, 0, V);
		//xf::cv::addWeighted(Y, saturation, img_128, 1 - saturation, 0, Y);

		xf::cv::Threshold<THRESH_TRUNC_T, XF_8UC1, HEIGHT, WIDTH, NPIX>(U, U, img_clip, img_clip);
		xf::cv::Threshold<THRESH_TOZERO_T, XF_8UC1, HEIGHT, WIDTH, NPIX>(U, U, 0, img_clip);

		xf::cv::Threshold<THRESH_TRUNC_T, XF_8UC1, HEIGHT, WIDTH, NPIX>(V, V, img_clip, img_clip);
		xf::cv::Threshold<THRESH_TOZERO_T, XF_8UC1, HEIGHT, WIDTH, NPIX>(V, V, 0, img_clip);

		//xf::cv::Threshold<THRESH_TRUNC_T, XF_8UC1, HEIGHT, WIDTH, NPIX>(Y, Y, img_clip, img_clip);
	    //xf::cv::Threshold<THRESH_TOZERO_T, XF_8UC1, HEIGHT, WIDTH, NPIX>(Y, Y, 0, img_clip);
	/*	xf::cv::Mat color_U_V[2];
		color_U_V[0] = U;
		color_U_V[1] = V;*/

		xf::cv::merge<XF_8UC1, XF_8UC3, HEIGHT, WIDTH, NPC>(Y, U, V, out_U_V);

		//channel_combine_accel(U, V, out_U_V);
		//xf::cv::merge(color_U_V, 2, out_U_V);
		//return out_U_V;
	}

}
