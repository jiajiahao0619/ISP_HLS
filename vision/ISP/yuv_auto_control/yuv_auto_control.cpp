#include<iostream>
#include<cmath>
#include<math.h>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;


Mat yuv_auto_control(Mat U, Mat V, int img_clip = 255, float hue_threshold = 1.77, float shifting = 0.25,int gamma =40)
{
	int img_rows = U.rows;
	int img_cols = U.cols;
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


int main()
{
	string path = "C:/Users/wpp/Desktop/工作/isp-master";
	//string image_name = "meat";
	//Mat img = imread(path + "/images/images/" + image_name + ".jpg");
	string image_name = "meat_meitu_4";
	Mat img = imread(path + "/images/images_pian/" + image_name + ".jpg");

	if (img.empty())
	{
		printf("%s\n", "读入图片失败；");
		system("pause");
		return 0;
	}
	imshow("原始图像", img);
	waitKey(0);
	Mat gray_img;
	Mat yuv_img;
	cvtColor(img, yuv_img, CV_RGB2YUV);
	Mat color_corrected[3];
	cv::split(yuv_img, color_corrected);
	Mat Y = color_corrected[0];
	Mat U = color_corrected[1];
	Mat V = color_corrected[2];


	int clip = 255;
	int hue_threshold = 1.77;
	float shifting = 0.25;
	Mat out_yuv_auto;
	Mat out_yuv_manual;
	Mat U_auto, V_auto;
	Mat U_manual, V_manual;

	Mat out_U_V;
	Mat color_U_V[2];
	out_U_V = yuv_auto_control(U, V);

	cv::split(out_U_V, color_U_V);

	color_corrected[0] = Y;
	color_corrected[1] = color_U_V[0];
	color_corrected[2] = color_U_V[1];

	cout << (float)color_U_V[0].at<uchar>(0, 0) << endl;
	cout << (float)color_U_V[1].at<uchar>(0, 0) << endl;

	merge(color_corrected, 3, out_yuv_auto);

	Mat out_yuv_auto1;
	cvtColor(out_yuv_auto, out_yuv_auto1, CV_YUV2BGR);
	cv::split(out_yuv_auto1, color_corrected);


	Mat R = color_corrected[0];
	Mat G = color_corrected[1];
	Mat B = color_corrected[2];

	cout << (float)color_corrected[0].at<uchar>(0, 0) << endl;
	cout << (float)color_corrected[1].at<uchar>(0, 0) << endl;
	cout << (float)color_corrected[2].at<uchar>(0, 0) << endl;

	Mat out_yuv_auto2;
	Mat color_corrected1[3];
	color_corrected1[2] = color_corrected[0];
	color_corrected1[1] = color_corrected[1];
	color_corrected1[0] = color_corrected[2];
	merge(color_corrected1, 3, out_yuv_auto2);

	imshow("修改后的图像_自动", out_yuv_auto2);
	//imshow("修改后的图像_手动", out_yuv_manual);
	waitKey(0);
	return 0;
}