#include<iostream>
#include<cmath>
#include<math.h>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;

Mat GC(Mat img, float gamma = 0.5, int clip = 255)
{
	Mat yuv_img;
	Mat out_yuv;
	Mat out_bgr;
	Mat color_corrected[3];
	cvtColor(img, yuv_img, COLOR_BGR2YUV);
	cv::split(yuv_img, color_corrected);
	Mat Y = color_corrected[0];
	Mat Y_32F;
	Y.convertTo(Y_32F, CV_32F);
	uchar T[256];
	for (int i = 0; i < Y_32F.rows; i++)
	{
		for (int j = 0; j < Y_32F.cols; j++)
		{
			Y.at<uchar>(i, j) = pow((Y_32F.at<float>(i, j)) / clip, gamma)*clip;
		}
	}
	color_corrected[0] = Y;
	merge(color_corrected, 3, out_yuv);
	cvtColor(out_yuv, out_bgr, COLOR_YUV2BGR);
	imwrite("C:/SICS/study/search/YUV/out_GC_rgb_me.jpg", out_bgr);
	return out_bgr;
}

int main()
{
	Mat img = imread("C:/SICS/study/search/YUV/test_5.jpg");
	if (img.empty())
	{
		printf("%s\n", "读入图片失败；");
		system("pause");
		return 0;
	}
	namedWindow("原始图像", 0);
	imshow("原始图像", img);
	waitKey(0);
	cout << img.rows << "  " << img.cols << endl;

	Mat out = GC(img);
	namedWindow("修改后的图像", 0);
	imshow("修改后的图像", out);
	waitKey(0);
	return 0;
}