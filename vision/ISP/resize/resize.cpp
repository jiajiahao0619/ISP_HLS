#include<iostream>
#include<cmath>
#include<math.h>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;

cv::Mat org, dst, img, tmp, large_img;
int drawing = 0;

Mat enlarge_img(Mat src, int scaleW, int scaleH, int method = 1)
{
	int  width = (src.cols)*scaleW;
	int  height = (src.rows)*scaleH;
	if (method == 1)// Nearest Neighbor Interpolation 
	{
		resize(src, large_img, Size(width, height), 0, 0, 1);
	}
	else if (method == 2)// Bilinear interpolation 
	{
		resize(src, large_img, Size(width, height), 0, 0, 2);
	}
	else if (method == 3)// Area Interpolation
	{
		resize(src, large_img, Size(width, height), 0, 0, 3);
	}
	else
	{
		printf("%s\n", "读入模式失败（请在模式1,2,3中进行选择）；");
		system("pause");
	}
	return large_img;
}
void on_mouse(int event, int x, int y, int flags, void *ustc)//event鼠标事件代号，x,y鼠标坐标，flags拖拽和键盘操作的代号  
{
	static Point pre_pt = Point(-1, -1);//初始坐标  
	static Point cur_pt = Point(-1, -1);//实时坐标  
	char temp[16];
	if (event == CV_EVENT_LBUTTONDOWN)//左键按下，读取初始坐标，并在图像上该点处划圆  
	{
		org.copyTo(img);//将原始图片复制到img中
		pre_pt = Point(x, y);
		circle(img, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);//划圆  
		imshow("img", img);
		drawing = 1;
	}
	else if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))//左键没有按下的情况下鼠标移动的处理函数  
	{
		img.copyTo(tmp);//将img复制到临时图像tmp上，用于显示实时坐标  
		cur_pt = Point(x, y);
		imshow("img", tmp);
	}
	else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))//左键按下时，鼠标移动，则在图像上划矩形  
	{
		img.copyTo(tmp);
		cur_pt = Point(x, y);
		rectangle(tmp, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);//在临时图像上实时显示鼠标拖动时形成的矩形  
		imshow("img", tmp);
	}
	else if (event == CV_EVENT_LBUTTONUP)//左键松开，将在图像上划矩形  
	{
		if (drawing == 1)
		{
			drawing = 0;
			org.copyTo(img);
			cur_pt = Point(x, y);
			circle(img, pre_pt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);
			rectangle(img, pre_pt, cur_pt, Scalar(0, 255, 0, 0), 1, 8, 0);//根据初始点和结束点，将矩形画到img上  
			imshow("img", img);
			img.copyTo(tmp);
			//截取矩形包围的图像，并保存到dst中  
			int width = abs(pre_pt.x - cur_pt.x);
			int height = abs(pre_pt.y - cur_pt.y);
			dst = org(Rect(min(cur_pt.x, pre_pt.x), min(cur_pt.y, pre_pt.y), width, height));
			namedWindow("dst");
			imshow("dst", dst);
			waitKey(0);

			int scaleW = 5;//放大倍数
			int scaleH = 5;
			int method = 3;//选择方法
			large_img = enlarge_img(dst, scaleW, scaleH, method);
			namedWindow("large_img");
			imshow("large_img", large_img);
			waitKey(0);
		}



	}

}


int main()
{
	string image_name = "meat1";
	string path = "c:/users/wpp/desktop/工作/isp-master";
	org = imread(path + "/images/images/" + image_name + ".jpg");
	org.copyTo(img);
	org.copyTo(tmp);
	namedWindow("img");//定义一个img窗口  
	setMouseCallback("img", on_mouse, 0);//调用回调函数  在on_mouse里面选择方式1,2,3
	imshow("img", img);
	waitKey(0);
}
