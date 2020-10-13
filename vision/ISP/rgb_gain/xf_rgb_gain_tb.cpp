#include "xf_rgb_gain_config.h"
#include "common/xf_headers.hpp"

#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        fprintf(stderr,"Invalid Number of Arguments!\nUsage:\n");
        fprintf(stderr,"<Executable Name> <input image path> \n");
        return -1;
    }
    cv::Mat in_img;
    cv::Mat Rimg,Gimg,Bimg,merge_img,diff;

    in_img.create(cv::Size(WIDTH,HEIGHT),CV_8UC3);
	merge_img.create(cv::Size(WIDTH,HEIGHT),CV_8UC3); // create memory for output image
	diff.create(cv::Size(WIDTH,HEIGHT),CV_8UC3);    // create memory for difference image

    //cv::Mat channels[3];
    vector<cv::Mat> channels;
    float rg=0.5;
    float gg=0.4;
    float bg=1.2;
    float r_result,g_result,b_result;

    in_img = cv::imread(argv[1],1);
    if (in_img.data == NULL)
    {
        fprintf(stderr,"cannot open image at %s\n",argv[1]);
        return 0;    
    }



    cv::split(in_img,channels);

    Rimg = channels.at(0);
    Gimg = channels.at(1);
    Bimg = channels.at(2);
    //vector<unsigned char> array(in_img.rows*in_img.cols);
    //if (in_img.isContinuous())
    //    array = Rimg.data;

    for(int j=0;j<in_img.rows;j++)
    {
    	for(int i=0;i<in_img.cols;i++)
    	{
    		//Rimg.at<cv::Vec3b>(j,i)[0] = (unsigned char)((float)Rimg.at<cv::Vec3b>(j,i)[0]*rg);
    		//Rimg.at<cv::Vec3b>(j,i)[0] = 0;
    		r_result = Rimg.at<unsigned char>(j,i)*rg;
    		g_result = Gimg.at<unsigned char>(j,i)*gg;
    		b_result = Bimg.at<unsigned char>(j,i)*bg;
    		Rimg.at<unsigned char>(j,i) = r_result;
    		Gimg.at<unsigned char>(j,i) = g_result;
    		Bimg.at<unsigned char>(j,i) = b_result;
    		r_result = 0;


    	}

    }
    //cv::Mat newchannel[3]={Bimg,Gimg,Rimg};
    //cv::merge(newchannel,merge_img);
    channels.at(0) = Rimg;
    channels.at(1) = Gimg;
    channels.at(2) = Bimg;
    cv::merge(channels,merge_img);
    //for
    printf("Rimg channels=%0d\n",Rimg.channels());
    cv::imwrite("merge.jpg",merge_img);
    cv::imwrite("out_Rimg.jpg",Rimg);
    cv::imwrite("out_Rimg.jpg",Gimg);
    cv::imwrite("out_Rimg.jpg",Bimg);

    static xf::cv::Mat<XF_8UC3,HEIGHT,WIDTH,NPC>imgInput(HEIGHT,WIDTH);
    static xf::cv::Mat<XF_8UC3,HEIGHT,WIDTH,NPC>imgOutput(HEIGHT,WIDTH);
    imgInput.copyTo(in_img.data);

    rgb_gain_accel(imgInput,imgOutput,rg,gg,bg);

    xf::cv::imwrite("out_img.jpg",imgOutput);
    //compare
    printf("imgOutput.rows=%0d,diff.rows=%0d\n",imgOutput.rows,diff.rows);
    xf::cv::absDiff(merge_img,imgOutput,diff);
    float err_per;
    xf::cv::analyzeDiff(diff,0,err_per);
    cv::imwrite("diff_img.jpg",diff);

    in_img.~Mat();
    Rimg.~Mat();
    Gimg.~Mat();
    Bimg.~Mat();
    diff.~Mat();

	if(err_per > 0.0f)
	{
	    printf("DIFF error");
		return 1;
	}
	printf("PASS\n");
	return 0;




        
}
