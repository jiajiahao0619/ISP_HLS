#include "common/xf_headers.hpp"
#include "xf_crop_resize_config.h"

int main(int argc, char** argv) {

	cv::Mat img, out_img[NUM_ROI], ocv_ref, error, diff[NUM_ROI], out_img1, ocv_ref1;

    if (argc != 2) {
        printf("Usage : <executable> <input image> \n");
        return -1;
    }


#if GRAY
    img.create(cv::Size(WIDTH, HEIGHT), CV_8UC1);
#else
    img.create(cv::Size(WIDTH, HEIGHT), CV_8UC3);
#endif

#if GRAY
    // reading in the color image
    img = cv::imread(argv[1], 0);
#else
    img = cv::imread(argv[1], 1);
#endif

    if (!img.data) {
        return -1;
    }

    cv::imwrite("input.png", img);
    
    unsigned int x_loc[NUM_ROI];
	unsigned int y_loc[NUM_ROI];
	unsigned int ROI_height[NUM_ROI];
	unsigned int ROI_width[NUM_ROI];
    
    x_loc[0]=500;
	y_loc[0]=500;
	ROI_height[0]=200;
	ROI_width[0]=200;
    
    for(int i=0; i< NUM_ROI; i++)
	{
#if GRAY
		out_img[i].create(ROI_height[i],ROI_width[i],img.depth());
		ocv_ref[i].create(ROI_height[i],ROI_width[i],img.depth());
		error.create(ROI_height[i],ROI_width[i],img.depth());
#else
		out_img[i].create(ROI_height[i],ROI_width[i],CV_8UC3);
		ocv_ref.create(ROI_height[i],ROI_width[i],CV_8UC3);
		error.create(ROI_height[i],ROI_width[i],CV_8UC3);
#endif

	}
	////////////////  reference code  ////////////////

    cv::Rect ROI(x_loc[0],y_loc[0],ROI_width[0],ROI_height[0]);
    ocv_ref=img(ROI);
    
    ////////////////////// HLS TOP function call ////////////////////////////

    xf::cv::Mat<TYPE, HEIGHT, WIDTH, NPC> imgInput(img.rows,img.cols);
    xf::cv::Mat<TYPE, HEIGHT, WIDTH, NPC> crpOutput;//(ROI_height,ROI_width);
    xf::cv::Mat<TYPE, HEIGHT, WIDTH, NPC> imgOutput;//(ROI_height,ROI_width);

    int roi[4];

    roi[0]=x_loc[0];
    roi[1]=y_loc[0];
    roi[2]=ROI_height[0];
    roi[3]=ROI_width[0];

	crpOutput.rows=roi[2];
	crpOutput.cols=roi[3];
	imgOutput.rows=1080;
	imgOutput.cols=1920;


	imgInput.copyTo(img.data);
	crop_resize_accel(imgInput,crpOutput,imgOutput,roi);
    
    
    //

    float err_per;
    printf("err_per= %0f\n",err_per);
    xf::cv::absDiff(ocv_ref, crpOutput, error);
    xf::cv::analyzeDiff(error, 7, err_per);
	xf::cv::imwrite("hls_out.png", imgOutput);
	xf::cv::imwrite("crop_out.png", crpOutput);
    cv::imwrite("ocv_strg.png", ocv_ref);  // reference image
    cv::imwrite("error.png", error);
    /*
    if (err_per > 0.0f) {
        printf("\nTest Failed\n");
        return -1;
    }
	*/
    return 0;

    
    
    

}    
