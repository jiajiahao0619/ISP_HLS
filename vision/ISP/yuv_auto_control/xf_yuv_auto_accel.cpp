#include "xf_yuv_auto_config.h"

Mat yuv_auto_control(xf::Mat<XF_16FC1, HEIGHT, WIDTH, NPC_T> &_U, xf::Mat<XF_16FC1, HEIGHT, WIDTH, NPC_T> &_V, int img_clip = 255, float hue_threshold = 1.77, float shifting = 0.25,int gamma =40)
{
	int img_rows = _U.rows;
	int img_cols = _U.cols;
	float sum_theta;
	float sum_count;
	float img_mean;
	//16FC1
	xf::Mat<XF_16FC1,HEIGHT,WIDTH,NPC_T>u_16f(img_rows,img_cols);
	xf::Mat<XF_16FC1,HEIGHT,WIDTH,NPC_T>v_16f(img_rows,img_cols);
	xf::Mat<XF_16FC1,HEIGHT,WIDTH,NPC_T>theta(img_rows,img_cols);
	//8UC1
	xf::Mat<XF_8UC1,HEIGHT,WIDTH,NPC_T>theta_8uc1(img_rows,img_cols);
	xf::Mat<XF_8UC1,HEIGHT,WIDTH,NPC_T>theta_TOZERO_INV(img_rows,img_cols);
	xf::Mat<XF_8UC1,HEIGHT,WIDTH,NPC_T>theta_TOZERO(img_rows,img_cols);    
	xf::Mat<XF_8UC1,HEIGHT,WIDTH,NPC_T>theta_BINARY(img_rows,img_cols);        
	for (int i=0; i<img_rows; i++)
    {
        for (int j=0; j<img_cols; j++)
        {
            u_16f.data[i*img_cols+j] = _U.data[i*img_cols+j] - 128.0;
            v_16f.data[i*img_cols+j] = _V.data[i*img_cols+j] - 128.0;
        }
        
    }
    //XF_DEGREES return results in Q10.6 range is(0,360)
    xf::phase<XF_DEGREES,XF_16SC1,XF_16SC1,HEIGHT, WIDTH,NPC1>(u_16f, v_16f,theta);
    //data type convert
    xf::converTo<XF_16UC1, XF_8UC1,HEIGHT,WIDTH,NPC_T>(theta,theta_8uc1,XF_CONVERT_16U_TO_8U,0);
    //threshold
    xf::threshold<XF_THRESHOLD_TYPE_TOZERO_INV,XF_8UC1,HEIGHT,WIDTH,NPC_T>(theta_8uc1,theta_TOZERO_INV,hue_threshold + shifting,0);
    xf::threshold<XF_THRESHOLD_TYPE_TOZERO,XF_8UC1,HEIGHT,WIDTH,NPC_T>(theta_TOZERO_INV,theta_TOZERO,hue_threshold - shifting,0);        
	//sum
	xf::sum<XF_8UC1,HEIGHT,WIDTH,NPC_T>(theta_TOZERO,sum_theta);
	//threshold
	xf::threshold<XF_THRESHOLD_TYPE_BINARY,XF_8UC1,HEIGHT,WIDTH,NPC_T>(theta_TOZERO,theta_BINARY,0,1);   
	//sum
	xf::sum<XF_8UC1,HEIGHT,WIDTH,NPC_T>(theta_BINARY,sum_count); 
	//
	img_mean = sum_theta / sum_count;            
	
	
	Mat U_32F, V_32F;
	U.convertTo(U_32F, CV_32F);
	V.convertTo(V_32F, CV_32F);
	Mat img_128(img_rows, img_cols, CV_32F, 128);
	Mat theta(img_rows, img_cols, CV_32F, 0.00); 

	phase(U_32F - img_128, V_32F - img_128, theta);// 色度像素点色度,atan2
	threshold(theta, theta, hue_threshold + shifting, 0, THRESH_TOZERO_INV);
	threshold(theta, theta, hue_threshold - shifting, 0, THRESH_TOZERO);

	float sum_theta = cv::sum(theta)[0];
	Mat sum_counnt_mat;
	threshold(theta, sum_counnt_mat, 0, 1, THRESH_BINARY);
	float sum_count = cv::sum(sum_counnt_mat)[0];
	float img_mean = sum_theta / sum_count;

	float cos = std::cos(img_mean - hue_threshold);
	float sin = std::sin(img_mean - hue_threshold);


	addWeighted(U, cos, V, sin, 128 * (1 - cos - sin), U);
	addWeighted(V, cos, U, -sin, 128 * (1 - cos + sin), V);

	//k = np.mean(np.sqrt(np.square(v - img_128) + np.square(u - img_128)))  # 饱和度
	//有问题，pdf里面的magnitude要求16S,这里要求32F，这个可以实现求距离sqrt(square(v - img_128) +square(u - img_128))
	Mat U_V_square;
	cv::magnitude((U_32F - img_128), (V_32F - img_128), U_V_square);
	float k = mean(U_V_square)[0];
	float beta_u = cv::sqrt(2) * gamma / k;
	float beta_v = cv::sqrt(2) * gamma / k;
	addWeighted(U, beta_u, U, 0, 128 * (1 - beta_u), U);
	addWeighted(V, beta_v, V, 0, 128 * (1 - beta_v), V);


	threshold(U, U, img_clip, img_clip, THRESH_TRUNC);
	threshold(U, U, 0, img_clip, THRESH_TOZERO);

	threshold(V, V, img_clip, img_clip, THRESH_TRUNC);
	threshold(V, V, 0, img_clip, THRESH_TOZERO);

	Mat out_U_V;
	Mat color_U_V[2];
	color_U_V[0] = U;
	color_U_V[1] = V;

	merge(color_U_V, 2, out_U_V);
	return out_U_V;
}