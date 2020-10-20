#include<iostream>
#include<cmath>
#include<math.h>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;

Mat yuv_auto_control(Mat U, Mat V, int mode = 3, int img_clip = 255, float hue_threshold = 1.77, float shifting = 0.25, int sat = 40)
{
	//控制变量格式可以修改convertTo的结果
	int img_rows = U.rows;
	int img_cols = U.cols;
	Mat U_32F, V_32F;
	U.convertTo(U_32F, CV_32F);
	V.convertTo(V_32F, CV_32F);
	Mat img_128(img_rows, img_cols, CV_32F, 128);

	//自动调色
	if (mode == 1 or mode == 3)
	{
		Mat theta(img_rows, img_cols, CV_32F, 0.00);

		//phase()输入输出均为16SC1
		phase(U_32F - img_128, V_32F - img_128, theta);

		//threshold()输入输出均为8UC1
		threshold(theta, theta, hue_threshold + shifting, 0, THRESH_TOZERO_INV);
		threshold(theta, theta, hue_threshold - shifting, 0, THRESH_TOZERO);

		//sum()输入为8UC1，输出为double
		float sum_theta = cv::sum(theta)[0];
		Mat sum_counnt_mat;
		threshold(theta, sum_counnt_mat, 0, 1, THRESH_BINARY);
		float sum_count = cv::sum(sum_counnt_mat)[0];
		float img_mean = sum_theta / sum_count;

		float cos = std::cos(img_mean - hue_threshold);
		float sin = std::sin(img_mean - hue_threshold);

		U = cos * U + sin * V + 128 * (1 - cos - sin);
		V = cos * V - sin * U + 128 * (1 - cos + sin);
		//addWeighted()输入输出均为8UC1
		//addWeighted(U, cos, V, sin, 128 * (1 - cos - sin), U);
		//addWeighted(V, cos, U, -sin, 128 * (1 - cos + sin), V);
	}

	//自动调饱和度
	if (mode == 2 or mode == 3)
	{
		Mat U_V_square;
		//magnitude()输入输出均为16SC1
		cv::magnitude((U_32F - img_128), (V_32F - img_128), U_V_square);
		//meanStdDev()输入为8UC1或8UC3，输出为16bit数据
		float k = mean(U_V_square)[0];
		float beta_u = cv::sqrt(2) * sat / k;
		float beta_v = cv::sqrt(2) * sat / k;
		U = beta_u * U + 128 * (1 - beta_u);
		V = beta_v * V + 128 * (1 - beta_v);
		//addWeighted(U, beta_u, U, 0, 128 * (1 - beta_u), U);
		//addWeighted(V, beta_v, V, 0, 128 * (1 - beta_v), V);
	}

	//将数据转化为uint8格式
	Mat U_8U, V_8U;
	U.convertTo(U_8U, CV_8U);
	V.convertTo(V_8U, CV_8U);

	//返回UV分量
	Mat temp_U_V[2];
	temp_U_V[0] = U_8U;
	temp_U_V[1] = V_8U;
	Mat merge_U_V;
	merge(temp_U_V, 2, merge_U_V);
	return merge_U_V;
}

int main()
{
	Mat img = imread("C:/SICS/study/search/YUV/test_1.jpg");
	if (img.empty())
	{
		printf("%s\n", "读入图片失败；");
		system("pause");
		return 0;
	}
	namedWindow("原始图像", 0);
	imshow("原始图像", img);
	waitKey(0);

	int clip = 255;
	float hue_threshold = 1.77;
	float shifting = 0.25;
	int mode = 3;//0对应不进行色度和饱和度的调整；1对应只进行色度的调整；2对应只进行饱和度的调整；3对应色度和饱和度均进行调整
	int sat = 40;

	//分别获取YUV分量
	Mat yuv_img;
	cvtColor(img, yuv_img, CV_RGB2YUV);
	Mat temp[3];
	cv::split(yuv_img, temp);
	Mat Y = temp[0];
	Mat U = temp[1];
	Mat V = temp[2];

	Mat out_U_V;
	out_U_V = yuv_auto_control(U, V, mode, clip, hue_threshold, shifting, sat);

	Mat temp_U_V[2];
	cv::split(out_U_V, temp_U_V);
	temp[0] = Y;
	temp[1] = temp_U_V[0];
	temp[2] = temp_U_V[1];

	Mat out_YUV;
	merge(temp, 3, out_YUV);

	Mat out_RGB;
	cvtColor(out_YUV, out_RGB, CV_YUV2BGR);
	cv::split(out_RGB, temp);

	Mat out_BGR;
	Mat color_corrected[3];
	color_corrected[2] = temp[0];
	color_corrected[1] = temp[1];
	color_corrected[0] = temp[2];
	merge(color_corrected, 3, out_BGR);

	namedWindow("修改后的图像_自动", 0);
	imshow("修改后的图像_自动", out_BGR);
	imwrite("C:/SICS/study/search/YUV/out.jpg", out_BGR);
	waitKey(0);
}