#include "xf_sat_ctrl_config.h"

void mat_split_uv(xf::cv::Mat<XF_8UC2, HEIGHT/2,WIDTH/2,NPC> &_src,
                  xf::cv::Mat<XF_8UC1, HEIGHT/2,WIDTH/2,NPC> &_u,
                  xf::cv::Mat<XF_8UC1, HEIGHT/2,WIDTH/2,NPC> &_v)
{
    ap_uint<16 * NPC> din;
    ap_uint<8 *NPC> u,v;
    mat_split_x:
    for(int x=0; x< _src.rows >> XF_BITSHIFT(NPC);x++){
        mat_split_y:
        for(int y=0; y< _src.cols >>XF_BITSHIFT(NPC);y++){
            din = _src.read(x*(_src.cols>>XF_BITSHIFT(NPC))+y);
            mat_split_i:
            for(int i=0; i<(1<<XF_BITSHIFT(NPC));i++){
                u(i*8+7,i*8) = din.range(15+i*16,8+i*16);
                v(i*8+7,i*8) = din.range(7+i*16,0+i*16);
            }
            _u.write((x*(_src.cols>>XF_BITSHIFT(NPC)+y))+y,u);
            _v.write((x*(_src.cols>>XF_BITSHIFT(NPC)+y))+y,v);
        }
    }                    
}

void mat_combine_uv(xf::cv::Mat<XF_8UC1,HEIGHT,WIDTH,NPC>&_u,
                    xf::cv::Mat<XF_8UC1,HEIGHT,WIDTH,NPC>&_v,
                    xf::cv::Mat<XF_8UC2,HEIGHT,WIDTH,NPC>&_dest)
{
    ap_uint<16*NPC> dout;
    ap_uint<8*NPC>u,v;
    for(int x=0;x<_dest.rows;x++){
        for(int y=0;y<( _dest.cols >> XF_BITSHIFT(NPC));y++){
            u = _u.read(x*(_dest.cols >>XF_BITSHIFT(NPC))+y);
            v = _v.read(x*(_dest.cols >>XF_BITSHIFT(NPC))+y);
            for(int i=0;i<(1<<XF_BITSHIFT(NPC));i++){
                dout(15 + i*16,i*16) = (u(i*8+7,i*8),
                                        v(i*8+7,i*8));
            }
            _dest.write((x*(_dest.cols >> XF_BITSHIFT(NPC))+y),dout);
            
        }
    }
    
}


void sat_ctrl_accel(xf::cv::Mat<XF_8UC1, HEIGHT, WIDTH, NPC>& U,
                    xf::cv::Mat<XF_8UC1, HEIGHT, WIDTH, NPC>& V,
                    xf::cv::Mat<XF_8UC2, HEIGHT, WIDTH, NPC>& UV,
                    unsigned char mode,
                    int sat)
{
//#pragma HLS DATAFLOW
    //8UC1
	xf::cv::Mat<XF_8UC1,HEIGHT,WIDTH,NPC>u_8u(HEIGHT,WIDTH);
//#pragma HLS ARRAY_MAP variable=u_8u horizontal
	xf::cv::Mat<XF_8UC1,HEIGHT,WIDTH,NPC>v_8u(HEIGHT,WIDTH);
//#pragma HLS STREAM variable=v_8u dim=1
    xf::cv::Mat<XF_8UC1,HEIGHT,WIDTH,NPC>uv_8u(HEIGHT,WIDTH);
//#pragma HLS STREAM variable=uv_8u dim=1
    //16SC1
	xf::cv::Mat<XF_16SC1,HEIGHT,WIDTH,NPC>u_16s(HEIGHT,WIDTH);
//#pragma HLS STREAM variable=u_16s dim=1
	xf::cv::Mat<XF_16SC1,HEIGHT,WIDTH,NPC>v_16s(HEIGHT,WIDTH);
//#pragma HLS STREAM variable=v_16s dim=1
    xf::cv::Mat<XF_16SC1,HEIGHT,WIDTH,NPC>uv_16s(HEIGHT,WIDTH);
//#pragma HLS STREAM variable=uv_16s dim=1
    //16SC1
	xf::cv::Mat<XF_32FC1,HEIGHT,WIDTH,NPC>u_32f(HEIGHT,WIDTH);
//#pragma HLS STREAM variable=u_32f dim=1
	xf::cv::Mat<XF_32FC1,HEIGHT,WIDTH,NPC>v_32f(HEIGHT,WIDTH);
//#pragma HLS STREAM variable=v_32f dim=1
    //data type convert
    xf::cv::convertTo<XF_8UC1, XF_16SC1,HEIGHT,WIDTH,NPC>(U,u_16s,XF_CONVERT_8U_TO_16S,0);
    xf::cv::convertTo<XF_8UC1, XF_16SC1,HEIGHT,WIDTH,NPC>(V,v_16s,XF_CONVERT_8U_TO_16S,0);
    //xf::cv::convertTo;


    //unsigned short mean[0] = (unsigned short*)malloc(sizeof(unsigned short));
    //unsigned short stddev[0] = (unsigned short*)malloc(sizeof(unsigned short));
    unsigned short mean[1] ;
    unsigned short stddev[1] ;
    mean[0] = 0; stddev[0]=0;
    //data sub 128
    for (int i=0; i<HEIGHT; i++)
    {
        for (int j=0; j<WIDTH; j++)
        {
            u_16s.data[i*WIDTH+j] = U.data[i*WIDTH+j] - 128;
            v_16s.data[i*WIDTH+j] = V.data[i*WIDTH+j] - 128;
            u_32f.data[i*WIDTH+j] = u_16s.data[i*WIDTH+j];
            v_32f.data[i*WIDTH+j] = v_16s.data[i*WIDTH+j];
        }
        
    }
    //

    xf::cv::magnitude<XF_L1NORM, XF_16SC1, XF_16SC1, HEIGHT, WIDTH, NPC>(u_16s, v_16s, uv_16s);
    /*
    for (int i=0; i<HEIGHT; i++)
    {
#pragma HLS DATAFLOW
        for (int j=0; j<WIDTH; j++)
        {
#pragma HLS DATAFLOW
            u_32f.data[i*WIDTH+j] = u_16s.data[i*WIDTH+j];
            v_32f.data[i*WIDTH+j] = v_16s.data[i*WIDTH+j];
        }

    }*/

    //mean need 8uc1 input
    xf::cv::convertTo<XF_16SC1, XF_8UC1,HEIGHT,WIDTH,NPC>(uv_16s,uv_8u,XF_CONVERT_16S_TO_8U,0);
    xf::cv::meanStdDev<XF_8UC1, HEIGHT, WIDTH, NPC>(uv_8u, mean, stddev);
    //float mean_f = (float) mean;
    float beta = 1.414 * sat / (float)mean[0] ;
    float bias = 128.0 -128.0*beta;
    for (int i=0; i<HEIGHT; i++)
    {
        for (int j=0; j<WIDTH; j++)
        {
        	u_32f.data[i*WIDTH+j] = u_32f.data[i*WIDTH+j]*beta + bias;
        	v_32f.data[i*WIDTH+j] = v_32f.data[i*WIDTH+j]*beta + bias;
        	u_8u.data[i*WIDTH+j] = (unsigned char) u_32f.data[i*WIDTH+j];
        	v_8u.data[i*WIDTH+j] = (unsigned char) v_32f.data[i*WIDTH+j];
        }        
    }
    mat_combine_uv(u_8u,v_8u,UV);
	
}
