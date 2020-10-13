#include "xf_yuv_auto_config.h"

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