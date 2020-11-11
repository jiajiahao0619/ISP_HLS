#include "xf_mirror_config.h"
#include "common/xf_headers.hpp"

#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include<vector>
//using namespace cv;
using namespace std;

cv::Mat mirror(cv::Mat img, int mirror_type){
    cv::Mat out;
    int img_rows = img.rows;
    int img_cols = img.cols;


    int img_rows_2 = img.rows / 2;
    int img_cols_2 = img.cols / 2;
    cv::Mat X(img_rows, img_cols, CV_32F, 0.00);
    cv::Mat Y(img_rows, img_cols, CV_32F, 0.00);

    if (mirror_type == 1)
    {
        for (int i = 0; i < img_rows; i++)//涓婁笅
        {
            for (int j = 0; j < img_cols; j++)
            {
                Y.at<float>(i, j) = img_rows - i;
                X.at<float>(i, j) = j;
            }
        }
    }
    else if (mirror_type == 2)
    {
        for (int i = 0; i < img_rows; i++)//宸﹀彸
        {
            for (int j = 0; j < img_cols; j++)
            {
                Y.at<float>(i, j) = i;
                X.at<float>(i, j) = img_cols - j;
            }
        }
    }
    else if (mirror_type == 3)
    {
        for (int i = 0; i < img_rows; i++)//涓婁笅宸﹀彸
        {
            for (int j = 0; j < img_cols; j++)
            {
                Y.at<float>(i, j) = img_rows - i;
                X.at<float>(i, j) = img_cols - j;
            }
        }
    }
    else
    {
        cout << "wrong type" << endl;
    }

    cv::remap(img, out, X, Y, CV_INTER_LINEAR);


    return out;
}




int main(int argc, char** argv) {
    cv::Mat img;
    cv::Mat out,diff;


    if (argc != 2) {
        printf("Usage : <executable> <input image> \n");
        return -1;
    }

    img.create(cv::Size(WIDTH,HEIGHT),CV_8UC3);
    out.create(cv::Size(WIDTH,HEIGHT),CV_8UC3); // create memory for output image
    diff.create(cv::Size(WIDTH,HEIGHT),CV_8UC3);    // create memory for difference image


    //img = cv::imread("C:/128x1281.png",0);
    img = cv::imread(argv[1], 1);
    //cv::Mat out;
    int img_rows = img.rows;
    int img_cols = img.cols;
    cout << img_rows << endl;
    cout << img_cols << endl;



    if (img.empty())
    {
        printf("%s\n", "鐠囪鍙嗛崶鍓у婢惰精瑙﹂敍锟�");
        system("pause");
        return 0;
    }


    int correction_type = 2;

    static xf::cv::Mat<XF_8UC3, HEIGHT, WIDTH, NPC> imgInput(HEIGHT, WIDTH);
    static xf::cv::Mat<XF_8UC3, HEIGHT, WIDTH, NPC> imgOutput(HEIGHT, WIDTH);
    cout << 1 << endl;
    imgInput.copyTo(img.data);

    out = mirror(img,correction_type);
    cout << 2 << endl;
    xf_mirror_accel(imgInput,imgOutput, correction_type);
    cout << 3 << endl;
    //imshow("娣囶喗鏁奸崥搴ｆ畱閸ユ儳鍎�", out);
    //waitKey(0);

    //imwrite("diff.png", imgOutput);

    float err_per;
    xf::cv::absDiff(out, imgOutput, diff);
    cout << 4 << endl;
    xf::cv::analyzeDiff(diff, 1, err_per);
    cout << 5 << endl;
    xf::cv::imwrite("hls_out.png", imgOutput);
    cv::imwrite("resize_ocv.png", out);

    img.~Mat();
    out.~Mat();
    diff.~Mat();

    if(err_per > 0.0f)
    {
        printf("DIFF error");
        return 1;
    }
    printf("PASS\n");
    return 0;



}
